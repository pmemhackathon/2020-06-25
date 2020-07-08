#include <iostream>
#include <fstream>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/p.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/pool.hpp>
#include <libpmemobj++/transaction.hpp>
#include <libpmemobj++/container/vector.hpp>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include "trie.hpp"
#include "hashset.hpp"

/* A unique layoout of the peristent memory pool that we are going to use */
#define LAYOUT "find_words"

using pmem::obj::delete_persistent;
using pmem::obj::make_persistent;
using pmem::obj::p;
using pmem::obj::persistent_ptr;
using pmem::obj::pool;
using pmem::obj::pool_base;
using pmem::obj::transaction;

/* available find_words operations */
enum find_words_op {
	UNKNOWN_FIND_WORDS_OP,
	FIND_WORDS_SOLVE,
	FIND_WORDS_PRINT,

	MAX_FIND_WORDS_OP,
};

/* find_words operations strings */
const char *find_words_str[MAX_FIND_WORDS_OP] = {"", "solve", "print"};

/*
 * parse_find_words_op -- parses the operation string and returns matching
 * find_words_op
 */
find_words_op
parse_find_words_op(const char *str)
{
	for (int i = 0; i < MAX_FIND_WORDS_OP; ++i)
		if (strcmp(str, find_words_str[i]) == 0)
			return (find_words_op)i;

	return UNKNOWN_FIND_WORDS_OP;
}

/*
 * We are going to store a board in an easier to operate on structure instead of
 * a raw file. To represent a 2D matrix we need a 2D table allocated on
 * persistent memory. In a libpmemobj-cpp library, we do have a collection of
 * "persistent containers" which are the structures with an STL-like interface.
 * We can use either pmem::obj::array or pmem::obj::vector for our purposes.
 * However, we do not know in advance what is the size of the board and we have
 * to allocate it dynamically. It means that instead of a static array, we need
 * to use pmem::obj::vector. Unlike the std::vector, we must allocate the
 * container and attach it to the persistent_ptr (std::vector as a variable
 * allocated on the stack will be freed automatically) to avoid persistent memory
 * leaks. We end up with a structure defined below. To access a single cell, we
 * need to dereference persistent_ptr three times - one time from root object
 * (however, we can cache reference to the outer vector) and two times to access
 * underlying array itself. This operation is highly inefficient because how the
 * persistent_ptr is defined and the fact, that compiler cannot cache a pool ID
 * (one of the two persistent_ptr layout fields) as the optimization. It will be
 * solved once self relative pointers will be introduced in libpmemobj-cpp and
 * pmem::obj::vector will be optimized accordingly (scheduled for libpmemobj v1.11).
 * 
 * We are going to store found words in a persistent memory as well.
 * The structure for representing a word will be a vector of pairs (a pair will
 * represent a cell (x,y) on the board) and the list of the found words will be
 * represented by a vector of such a vector of pairs. All in all, we will have a
 * dynamically allocated 2D matrix again, this time every cell will be of the
 * std::pair type instead of a char.
 * 
 * One of the reasons why we are using persistent memory containers defined in
 * the libpmemobj-cpp instead of STL ones is the on-media layout of the latters.
 * STL (and C++ itself) does not guarantee a fixed layout of most of the
 * classes/structures. You can read more about the issue here:
 * https://pmem.io/2018/11/20/cpp-persistent-containers.html
 * https://pmem.io/2019/10/04/cpp-limitations.html
 * std::pair for trivial types is implemented in all the C++ standard libraries
 * in the same way and most of the well know compilers define this structure
 * layout exactly the same.
 * Using std::pair is theoreatically undefined behaviour, we could easly use
 * pmem::obj::vector with only 2 elements but in this example, we want to
 * accentuate a potential issues when programming on persistent memory.
 * In the libpmemobj-cpp persistent containers collection (v1.10) we are still
 * missing the implementation of pmem::obj::pair. There is only internal and
 * simplified implementation under include/libpmemobj++/detail/pair.hpp.
 * Any contribution is more than welcome.
 */
template<typename T>
using matrix_type = typename pmem::obj::vector<pmem::obj::vector<T>>;
using board_type = matrix_type<char>;
using result_type = matrix_type<std::pair<int,int>>;

/*
 * root_type - a main structure associated with persistent memory pool
 */
struct root_type {
	persistent_ptr<trie> trie_root;
	persistent_ptr<board_type> board;
	persistent_ptr<result_type> result;

	/*
	 * An indiactor where we should start the finding process after
	 * the crash happen. Initially zeroed, since the whole root strucutre is
	 * initially zeroed.
	 */
	p<int> progress_x;
	p<int> progress_y;
};

/*
 * Construct a trie and insert the words from a dictionary file.
 */
void create_trie(pool<root_type> &pop, const std::string &dictionary_file) {
	auto r = pop.root();

	if (r->trie_root != nullptr) {
		return; // trie is already created from the dictionary
	}

	// Construct a trie structure and add all the words from a dictionary
	// in a single transaction. It means that the trie will be created and
	// all the words will be added or trie won't be created at all (e.g. if
	// the crash happen).
	pmem::obj::transaction::run(pop, [&] {
		r->trie_root = make_persistent<trie>();

		std::ifstream dictionaryStream(dictionary_file);

		if (!dictionaryStream.is_open()) {
			throw std::runtime_error("Failed to open a board file");
		} else {
			std::string word;
			while (dictionaryStream >> word) {
				r->trie_root->insert(word);
			}
		}
	});
}

/*
 * Allocate 2D matrix in a persistent memory and fill it in with data set from
 * the board file.
 */
void load_board(pool<root_type> &pop, const std::string &board_file) {
	auto r = pop.root();

	if (r->board != nullptr) {
		return; // board is already loaded from the file
	}

	// Allocate a 2D vector board in a persistent memory and copy a data set
	// from a board file within a single transaction. It means that the 2D
	// vector will be allocated and all the cells will be copied from the
	// file or the board won't be created at all (e.g. if the crash happen).
	pmem::obj::transaction::run(pop, [&] {
		r->board = make_persistent<board_type>();
		board_type &board = *(r->board);

		std::ifstream boardStream(board_file);

		if (!boardStream.is_open()) {
			throw std::runtime_error("Failed to open a board file");
		} else {
			std::string line;
			while (std::getline(boardStream, line)) {
				int sz = 0;
				while (line[sz] >= 'a' && line[sz] <= 'z') sz++;
				board.emplace_back(sz);
				for (int i = 0; i < sz; ++i) {
					board.back()[i] = line[i];
				}
			}
		}
	});

}

/*
 * dfs -- deep-frist-search helper function, can be called only from solve()
 * 	  function.
 */
void dfs(const trie &trie_root, board_type &board, result_type &result,
	 std::string &curr_word, std::vector<std::pair<int,int>> curr_path,
	 int i, int j) {

	curr_path.push_back({i, j});
	curr_word += board[i][j];
	board[i][j] += 26; // mark as checked

	if (trie_root.find_prefix(curr_word)) { // it is existing prefix, continue dfs
		int N = board.size(), M = board[0].size();

		if(i - 1 >= 0 && board[i - 1][j] < 'a' + 26) {
			dfs(trie_root, board, result, curr_word, curr_path, i - 1, j);
		}
		if(j - 1 >= 0 && board[i][j - 1] < 'a' + 26) {
			dfs(trie_root, board, result, curr_word, curr_path, i, j - 1);
		}
		if(i + 1 < N && board[i + 1][j] < 'a' + 26) {
			dfs(trie_root, board, result, curr_word, curr_path, i + 1, j);
		}
		if(j + 1 < M && board[i][j + 1] < 'a' + 26) {
			dfs(trie_root, board, result, curr_word, curr_path, i, j + 1);
		}
	}

	board[i][j] -= 26; // restore previous value

	if (trie_root.find_word(curr_word)) { // it is a valid word; add to result
		result.emplace_back(curr_path.size());

		for (int k = 0; k < curr_path.size(); ++k) {
			result.back()[k] = curr_path[k];
		}
	}

	curr_word.pop_back();
	curr_path.pop_back();
}

/*
 * solve -- find all the words from a trie in a board matrix and save them in
 *	    a result vector
 */
void solve(pool<root_type> &pop) {
	auto r = pop.root();

	if (nullptr == r->board || nullptr == r->trie_root) {
		throw std::logic_error("Can't solve the problem, load the board and create trie first.");
	}

	if (nullptr == r->result) {
		pmem::obj::transaction::run(pop, [&] {
			r->result = make_persistent<result_type>();
		});
	}

	trie &trie_root = *(r->trie_root);
	board_type &board = *(r->board);
	result_type &result = *(r->result);

	int N = board.size(), M = board[0].size();

	for (int i = r->progress_x; i < N; ++i) {
		for (int j = r->progress_y; j < M; ++j) {
			pmem::obj::transaction::run(pop, [&] {
				std::string curr_word;
				std::vector<std::pair<int,int>> curr_path;

				dfs(trie_root, board, result, curr_word,
				    curr_path, i, j);

				if (j + 1 < M) {
					r->progress_y = j + 1;
				}
				else {
					r->progress_y = 0;
					r->progress_x = i + 1;
				}
			});
		}
	}
}

/*
 * Print the results calculated so far.
 */
void print_results(pool<root_type> &pop) {
	auto r = pop.root();

	if (nullptr == r->board || nullptr == r->trie_root || nullptr == r->result) {
		throw std::logic_error("Can't print results, solve the problem first.");
	}

	board_type &board = *(r->board);
	result_type &result = *(r->result);

	int N = board.size(), M = board[0].size();
	double completed = (((double)M * (double)r->progress_x + (double)r->progress_y)
			   / (double)(M * N)) * 100;
	std::cout << "Problem solved in " << completed << "%, results:" << std::endl;

	for (int i = 0; i < result.size(); ++i) {
		std::string curr_word;
		std::cout << i << ": ";
		for (int j = 0; j < result[i].size(); ++j) {
			int x = result[i][j].first;
			int y = result[i][j].second;
			curr_word += board[x][y];
			std::cout << "(" << x << "," << y << ") ";
		}
		std::cout << curr_word << std::endl;
	}
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		throw std::invalid_argument("usage: " + std::string(argv[0]) +
					    " pool-path [solve [dictionary-file board-file]|print] ");
	}

	const std::string pool_path = argv[1];

	pool<root_type> pop;

	if (access(argv[1], F_OK) == 0) {
		pop = pool<root_type>::open(pool_path, LAYOUT);
	} else {
		pop = pool<root_type>::create(pool_path, LAYOUT,
					PMEMOBJ_MIN_POOL, S_IWUSR | S_IRUSR);
	}

	find_words_op op = parse_find_words_op(argv[2]);
	switch (op) {
		case FIND_WORDS_SOLVE:
			if (argc == 5) {
				create_trie(pop, argv[3]);
				load_board(pop, argv[4]);
				solve(pop);
				break;
			}
		case FIND_WORDS_PRINT:
			print_results(pop);
			break;
		default:
			throw std::invalid_argument("invalid operation or arguments");
	}

	pop.close();

	return 0;
}
