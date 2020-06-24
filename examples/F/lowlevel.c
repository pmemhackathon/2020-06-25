#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <string.h>
#include <libpmem.h>

/* using 4k of pmem for this example */
#define PMEM_LEN 4096

int
main(int argc, char *argv[])
{
	char *pmemaddr;
	size_t mapped_len;
	int is_pmem;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	/*
	 * create the file and memory map it.  pmem_map_file() handles
	 * calling mmap() with the appropriate arguments and detecting
	 * if the file is really pmem.  it sets the flag "is_pmem" to
	 * tell us if we can flush from user space or if we have to
	 * use msync() to flush changes to persistence.
	 */
	if ((pmemaddr = pmem_map_file(argv[1], PMEM_LEN, PMEM_FILE_CREATE,
				0666, &mapped_len, &is_pmem)) == NULL)
		err(1, "pmem_map_file: %s", argv[1]);

	/* store a string to the persistent memory */
	char s[] = "This is new data written to the file";
	strcpy(pmemaddr, s);

	/*
	 * flush our string to persistence. since we check is_pmem to see
	 * if msync() is required, this program produces correct results
	 * on pmem and regular files (it is just slower on storage).
	 */
	if (is_pmem)
		pmem_persist(pmemaddr, sizeof(s));
	else
		pmem_msync(pmemaddr, sizeof(s));

	/*
	 * the above steps, copying something to memory and flushing it
	 * to persistence, are so common libpmem provides optimized ways
	 * to do it.
	 */
	if (is_pmem)
		pmem_memcpy_persist(pmemaddr, s, sizeof(s));
	else
		printf("can't use pmem_memcpy_persist since it is not pmem\n");

	/* unmap the file */
	pmem_unmap(pmemaddr, mapped_len);

	printf("done.\n");
	exit(0);
}
