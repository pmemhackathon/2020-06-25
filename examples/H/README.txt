Using pmemcheck

A C++ program using PMDK's libpmemobj is shown, but with several common
bugs in the code.  This example shows how to use pmemcheck to identify and
fix the bugs.

This example is intended for C++ programmers.

This is example consists of these files:

find_bugs.cpp       -- a sample C++ program containing some bugs
run.sh              -- run the find_bugs program under pmemcheck
Makefile            -- rules for building all the programs in this example
find_bugs_fixed.cpp -- the answers!

To build all the programs in this example run: make
To run it and see what it illustrates run: ./run.sh

See if you can look at the valgrind output and fix the bugs in the program.
