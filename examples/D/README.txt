Converting a C Program for Persistent Memory

In this example, we start by writing a simple C program to solve a basic
problem.  We then enhance the program to be multi-threaded.  Finally,
we change the program to use persistent memory through the libpmemobj
library in PMDK.

C and C++ programmers will get the most out of this example.
For persistent memory hacking in C, this example provides a good
starting point.

This is example consists of these files:

freq.c               -- a simple C program (not persistent) to start with
freq_mt.c            -- a multi-threaded version of freq.c (not persistent)
freq_pmem.c          -- converted to pmem, C version using libpmemobj
freq_pmem_print.c    -- a separate C program for printing results from pmem
run_freq.sh          -- run the freq example
run_freq_mt.sh       -- run the freq_mt example
run_freq_pmem.sh     -- run the freq_pmem example
Makefile             -- rules for building all the programs in this example
words.txt            -- just a small file with some words we can count

To build all the programs in this example run: make
To run them, use the run*.sh scripts (in the order listed above).

Modifying the code and run steps is a great way to learn from this example.

This example shows a simple task, counting the work frequency in a list of
text files, first implemented in C, then multi-threaded, then converted
to store the results persistently in pmem.

See the next example for a cleaner libpmemobj API which uses the C++
type system instead of the C macros you see here.
