Mapping Persistent Memory

This example shows how persistent memory is mapped into an application
using the mmap() system call on Linux.  This example is recommended
reading for everyone since all the other examples in this hackathon
build on this basic concept.

If your C coding skills are rusty -- don't worry!  We tell you everything
you need to know to get a basic mmap() program running.

This is example consists of these files:

basic_mmap.c -- the bare minimum mmap() code to map persistent memory
Makefile     -- rules for building this example
fs.out       -- sample output of "df" showing pmem on your system
run.sh       -- one way to run this example to illustrate what it does
windows.c    -- the windows version of basic_mmap.c, for reference

To build this example run: make
To run it and see what it illustrates run: ./run.sh

Modifying the code and run steps is a great way to learn from this example.

This example shows the bare-minumum, raw mmap() access using POSIX
calls.  This includes using the POSIX msync() to flush stores to
persistence.  This illustrates that POSIX interfaces work as expected,
but this isn't the fastest way to use persistent memory.  See Example F
for faster ways to flush changes from user space and details on libpmem
which makes the low-level raw access to persistent memory easier.

Now that you've seen the basic memory mapped of persistent memory,
continue on to more examples to see more programmer-friendly APIs
that build on this basic memory mapping mechanism.
