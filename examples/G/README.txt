Using pmem as Volatile Memory

It might be counter-intuitive to use persistent memory for a non-persistent
pool of memory, but many applications are now doing exactly that.  Persistent
memory can be cheaper and larger than normal DRAM, and accesses to it can
be slower.  Applications put the "hot" latency-sensitive data structures
in DRAM, and the less latency-sensitive data structures in pmem.  This allows
memory-hungry applications to see a much larger capacity for a lower cost,
while maintaining their performance goals.

This example is intended for C programmers.

This is example consists of these files:

volatile.c -- an example showing the volatile use of pmem
Makefile   -- rules for building this example
run.sh     -- one way to run this example to illustrate what it does

To build this example run: make
To run it and see what it illustrates run: ./run.sh

Modifying the code and run steps is a great way to learn from this example.

This example use the libmemkind library to provide the familiar malloc()
style dynamic memory allocation that C programmers are used to.  But memkind's
version of malloc() takes an extra argument that specifies which type of
memory is being allocated.

No persistence in this example, so no concerns about flushing to persistent
or transactions for failure atomicity, etc.
