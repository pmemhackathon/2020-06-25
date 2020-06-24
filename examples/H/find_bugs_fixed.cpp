/*
 * find_bugs.cpp -- program with few bugs related to persistent memory
 * 	programming. Can you find them?
 *
 * create the pool for this program using pmempool, for example:
 *	pmempool create obj --layout=find_bugs -s 1G find_bugs
 */

#include <libpmemobj++/container/array.hpp>
#include <libpmemobj++/containerexperimental/string.hpp>
#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/mutex.hpp>
#include <libpmemobj++/p.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/pext.hpp>
#include <libpmemobj++/pool.hpp>
#include <libpmemobj++/transaction.hpp>

static const std::string LAYOUT = "find_bugs";

struct data {
	data(): simple_variable(0), pmem_property(0), vec(10) {
		int_ptr = pmem::obj::make_persistent<pmem::obj::p<int>>(10);
	}

	pmem::obj::p<int> simple_variable;

	pmem::obj::p<int> pmem_property;

	pmem::obj::vector<int> vec;

	pmem::obj::persistent_ptr<pmem::obj::p<int>> int_ptr;
};

struct root {
	pmem::obj::persistent_ptr<data> ptr;
	pmem::obj::persistent_ptr<data> ptr2;
};

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "usage: " << argv[0] << " file-name" << std::endl;
		return 1;
	}

	auto path = argv[1];

	auto pop = pmem::obj::pool<root>::open(path, LAYOUT);

	auto r = pop.root();
	if (r->ptr == nullptr) {
		pmem::obj::transaction::run(pop, [&] {
			r->ptr = pmem::obj::make_persistent<data>();
			r->ptr2 = pmem::obj::make_persistent<data>();
		});
	}

	/*******************************  BUG 1  ******************************/

	pmem::obj::transaction::run(pop, [&]{
		r->ptr->simple_variable = 10;
	});

	/**********************************************************************/
	/*******************************  BUG 2  ******************************/

	pmem::obj::transaction::run(pop, [&]{
		auto &ref = r->ptr->vec[1];
		ref = 1;

		auto it = r->ptr->vec.begin() + 2;
		*it = 2;
	});

	/**********************************************************************/
	/*******************************  BUG 3  ******************************/

	pmem::obj::transaction::run(pop, [&]{
		r->ptr->vec.push_back(10);

		*(r->ptr->int_ptr) = 11;
	});

	/**********************************************************************/
	/*******************************  BUG 4  ******************************/

	pmem::obj::transaction::run(pop, [&]{
		r->ptr->vec[0] = 0;
	});

	/**********************************************************************/
	/*******************************  BUG 5  ******************************/

	/*
	 * The point of this bug is to show that pmemcheck can't detect
	 * consistency errors.
	 * In this case persistent_ptr is larger than 8 bytes which means that
	 * can't be safely updated without a transaction.
	 */
	pmem::obj::transaction::run(pop, [&]{
		r->ptr2->pmem_property = r->ptr->pmem_property;
		r->ptr2 = r->ptr;
	});

	/**********************************************************************/

	pop.close();

	return 0;
}
