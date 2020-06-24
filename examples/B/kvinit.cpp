#include <cassert>
#include <filesystem>
#include <iostream>
#include <libpmemkv.hpp>

using namespace pmem::kv;
using std::cerr;
using std::cout;
using std::endl;
using std::string;

// default size for this example: 10 Meg
const uint64_t SIZE = 10 * 1024 * 1024;

//
// must_open_or_create() is a convenience function to open the
// database if it already exists, or create it with a default
// size if it doesn't.
//
db *must_open_or_create(const char *path) {

	// start by creating the db object
	db *kv = new db();
	assert(kv != nullptr);

	// create the config information for the pmemkv open
	config cfg;

	if (cfg.put_string("path", path) != status::OK) {
		cerr << pmemkv_errormsg() << endl;
		exit(1);
	}

	if (!std::filesystem::exists(path)) {
		// file doesn't exist, so add config flags to create it
		if (cfg.put_uint64("force_create", 1) != status::OK) {
			cerr << pmemkv_errormsg() << endl;
			exit(1);
		}

		if (cfg.put_uint64("size", SIZE) != status::OK) {
			cerr << pmemkv_errormsg() << endl;
			exit(1);
		}
	}

	if (kv->open("cmap", std::move(cfg)) != status::OK) {
		cerr << errormsg() << endl;
		exit(1);
	}

	return kv;

}
