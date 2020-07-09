#include <iostream>
#include <libpmemobj++/container/array.hpp>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/p.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/pool.hpp>
#include <libpmemobj++/transaction.hpp>

using pmem::obj::delete_persistent;
using pmem::obj::make_persistent;
using pmem::obj::p;
using pmem::obj::persistent_ptr;
using pmem::obj::pool;
using pmem::obj::pool_base;
using pmem::obj::transaction;

/*
 * Basic implementation of Trie structure.
 * See https://en.wikipedia.org/wiki/Trie for details.
 */
class trie {

struct trieNode {
        // Note that we are declareing pmem::obj::array of children
        // instead of dynamic pmem::obj:vector so we have one dereference of
        // persistent_ptr when accessing children less.
        pmem::obj::array<persistent_ptr<trieNode>, 26> children;
        p<bool> is_word;

        trieNode() : is_word(false) {
                // Construct and initialize new object; make sure that the
                // structure instance is being allocated on persistent memory
                // and constructor is being called inside transaction scope to
                // guarantee consistency.
                if (nullptr == pmemobj_pool_by_ptr(this)) {
		        throw pmem::pool_error("Invalid pool handle.");
                }

                if (pmemobj_tx_stage() != TX_STAGE_WORK) {
		        throw pmem::transaction_scope_error("Function called out of transaction scope.");
                }
        }

        ~trieNode() {
                for (auto pptr: children) {
                        delete_persistent<trieNode>(pptr);
                }
        }
};

public:
        /*
         * Default constructor.
         */
        trie() {
                if (nullptr == pmemobj_pool_by_ptr(this)) {
		        throw pmem::pool_error("Invalid pool handle.");
                }

                if (pmemobj_tx_stage() != TX_STAGE_WORK) {
		        throw pmem::transaction_scope_error("Function called out of transaction scope.");
                }

                this->root = make_persistent<trieNode>();
        }

        /*
         * Check if given word is stored in a trie.
         */
        bool find_word(const std::string &word) const {
                return find_helper(word, FIND_WORD);
        }

        /*
         * Check if the word with a given prefix is stored in a trie.
         */
        bool find_prefix(const std::string &word) const {
                return find_helper(word, FIND_PREFIX);
        }

        /*
         * Insert a word to the trie.
         */
        void insert(const std::string &word) {
                // Insert a new value in transaction in order to guarantee
                // data consistency and atomicty. If we want to perform
                // transaction in a specyfic persistent memory pool, we need a
                // pool handle
                pool_base pb = pool_base(pmemobj_pool_by_ptr(this));

                persistent_ptr<trieNode> curr = this->root;
                transaction::run(pb, [&] {
                        for (char c: word) {
                                if (nullptr == curr->children[c - 'a']) {
                                        curr->children[c - 'a'] = make_persistent<trieNode>();
                                }
                                curr = curr->children[c - 'a'];
                        }
                        curr->is_word = true;
                });
        }

        /*
         * Print words stored in trie.
         */
        void print() {
                std::cout << "Words stored in a trie:" << std::endl;
                std::string curr_prefix;
                print_dfs(root, curr_prefix);
        }

private:
        persistent_ptr<trieNode> root;

        static const int FIND_WORD = 0;
        static const int FIND_PREFIX = 1;

        bool find_helper(const std::string &word, int flag) const {
                persistent_ptr<trieNode> curr = this->root;

                for (char c: word) {
                        if (nullptr == curr->children[c - 'a']) {
                                return false;
                        }
                        curr = curr->children[c - 'a'];
                }

                return flag ? true : (bool)curr->is_word;
        }

        void print_dfs(persistent_ptr<trieNode> n, std::string &curr) {
                trieNode &node = *(n);
                if (node.is_word) {
                        std::cout << curr << std::endl;
                }

                for (int i = 0; i < node.children.size(); ++i) {
                        if (nullptr != node.children[i]) {
                                curr += 'a' + i;
                                print_dfs(node.children[i], curr);
                                curr.pop_back();
                        }
                }
        }
};
