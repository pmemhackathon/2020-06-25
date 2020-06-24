#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <memkind.h>

/* 16 Megs of pmem used for this example */
#define PMEM_SIZE (16 * 1024 * 1024)

/* convenience routine to print error message and exit */
void memkind_fatal(int err)
{
	char error_message[MEMKIND_ERROR_MESSAGE_SIZE];

	memkind_error_message(err, error_message, MEMKIND_ERROR_MESSAGE_SIZE);
	fprintf(stderr, "%s\n", error_message);
	exit(1);
}

int main(int argc, char *argv[])
{
	struct memkind *pmem_kind;
	int err;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s [pmem_dir]\n", argv[0]);
		exit(1);
	}

	err = memkind_create_pmem(argv[1], PMEM_SIZE, &pmem_kind);
	if (err) {
		memkind_fatal(err);
	}

	/* simulate an application doing allocations of various sizes */
	char *allocations[100];
	for (int i = 0; i < 100; i++) {
		size_t sz = rand() % 5000 + 1;

		/*
		 * one way an application might decide where to place
		 * data is based on the size.  where an existing application
		 * calls malloc(), it could be modified to choose between
		 * DRAM and pmem as shown below.
		 */
		if (sz > 1024) {
			/* larger than 1k, use pmem */
			printf("allocate %zd bytes from pmem\n", sz);
			allocations[i] = memkind_malloc(pmem_kind, sz);
		} else {
			/* small allocation, use DRAM */
			printf("allocate %zd bytes from DRAM\n", sz);
			allocations[i] = memkind_malloc(MEMKIND_DEFAULT, sz);
		}
		if (allocations[i] == NULL) {
			fprintf(stderr, "out of memory (size %zd)\n", sz);
		}
	}

	/*
	 * now just use the allocations the same way, regardless
	 * of where they came from.
	 *
	 */

	/* ... */

	/*
	 * free the allocations.  note how we didn't have to keep track
	 * of where they came from, memkind_free() does that if you pass
	 * in NULL for the kind.
	 */
	for (int i = 0; i < 100; i++) {
		if (allocations[i] != NULL) {
			memkind_free(NULL, allocations[i]);
		}
	}

	err = memkind_destroy_kind(pmem_kind);
	if (err) {
		memkind_fatal(err);
	}

	exit(0);
}
