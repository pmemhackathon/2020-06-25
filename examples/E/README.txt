The Word Frequency Example in C++

This example takes the previous word frequency example that was
written in C, and converts it to cleaner C++ API for libpmemobj.

C and C++ programmers will get the most out of this example.
For persistent memory hacking in C++, this example provides a good
starting point.

This is example consists of these files:

freq_pmem_print.c  -- the printing program from the previous exercise
freq_pmem_cpp.cpp  -- a version of freq_pmem written in C++
run.sh             -- run the freq_pmem_cpp example
Makefile           -- rules for building all the programs in this example
words.txt          -- just a small file with some words we can count

To build all the programs in this example run: make
To run it and see what it illustrates run: ./run.sh

Modifying the code and run steps is a great way to learn from this example.

This example builds on the previous example, showing a simple word
frequency task where the results are stored in a persistent memory
data structure.  Each time the program is run, it add any new counts
to the already existing hash table in pmem.  Pass the program multiple
files on the command line to execute multiple threads concurrently.
