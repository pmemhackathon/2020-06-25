#include <iostream>
#include <libpmemkv.hpp>

using namespace pmem::kv;
using std::cerr;
using std::cout;
using std::endl;
using std::string;

db *must_open_or_create(const char *path);

//
// kvprint is a callback function to process each key/value pair.  it is
// used when we call kv->get_all() below.  the k and v arguments point
// to the key and value in-place in the persistent memory, not copies.
//
int kvprint(string_view k, string_view v) {

	cout << k.data() << "=\"" << v.data() << "\"" << endl;
	return 0;

}

//
// this is the main program, used this way:
//	pmemkv pmemfile           -- print all the keys and values in the pmemfile
//	pmemkv pmemfile key       -- lookup key and print the value
//	pmemkv pmemfile key value -- add a key/value pair to the pmemfile
//
// the pmemfile is created automatically if it doesn't already exist.
//
int main(int argc, char *argv[]) {

	// arguments must include file name and may include key value pairs
	if (argc < 2 || argc > 4) {
		cerr << "usage: " << argv[0] << " kvfile [key [value]]" << endl;
		exit(1);
	}

	db *kv = must_open_or_create(argv[1]);

	if (argc == 2) {

		// iterate through the key-value store, printing them
		kv->get_all(kvprint);

	} else if (argc == 3) {

		// lookup the given key and print the value
		auto ret = kv->get(argv[2], [&](string_view value) {
			cout << argv[2] << "=\"" << value.data() << "\"" << endl;
		});
		if (ret != status::OK) {
			cerr << errormsg() << endl;
			exit(1);
		}

	} else {

		// add the given key-value pair
		if (kv->put(argv[2], argv[3]) != status::OK) {
			cerr << errormsg() << endl;
			exit(1);
		}

	}

	// stop the pmemkv engine
	delete kv;

	exit(0);
}
