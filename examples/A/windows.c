#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Windows.h>

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	/* Create the file or open if the file already exists */
	HANDLE fh = CreateFile(argv[1],
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (fh == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "CreateFile, gle: 0x%08x", GetLastError());
		exit(1);
	}

	/* Get the file length for use when memory mapping later */
	DWORD filelen = GetFileSize(fh, NULL);
	if (filelen == 0) {
		fprintf(stderr, "GetFileSize, gle: 0x%08x", GetLastError());
		exit(1);
	}

	/* Create a file mapping object */
	HANDLE fmh = CreateFileMapping(fh,
		NULL, /* security attributes */
		PAGE_READWRITE,
		0,
		0,
		NULL);

	if (fmh == NULL) {
		fprintf(stderr, "CreateFileMapping, gle: 0x%08x",
			GetLastError());
		exit(1);
	}

	/* Map into our address space and get a pointer to the beginning */
	char *pmaddr = (char *)MapViewOfFileEx(fmh,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		filelen,
		NULL); /* hint address */

	if (pmaddr == NULL) {
		fprintf(stderr, "MapViewOfFileEx, gle: 0x%08x",
			GetLastError());
		exit(1);
	}

	/* write to persistent memory... */
	strcpy(pmaddr, "Hello, persistent memory!");

	/* Flush this page with length rounded up to 4k page size */
	if (FlushViewOfFile(pmaddr, 4096) == FALSE) {
		fprintf(stderr, "FlushViewOfFile, gle: 0x%08x",
			GetLastError());
		exit(1);
	}

	/* Now flush the complete file to backing storage */
	if (FlushFileBuffers(fh) == FALSE) {
		fprintf(stderr, "FlushFileBuffers, gle: 0x%08x",
			GetLastError());
		exit(1);
	}

	/* Explicitly unmap before closing the file */
	if (UnmapViewOfFile(pmaddr) == FALSE) {
		fprintf(stderr, "UnmapViewOfFile, gle: 0x%08x",
			GetLastError());
		exit(1);
	}

	CloseHandle(fmh);
	CloseHandle(fh);

	printf("done.\n");
	exit(0);
}
