Basic Persistent Memory Transactions

This example introduces the ideas of persistent memory transactions,
used to maintain consistent data structures in persistent memory even in
the face of a program or system crash.  We recommend that everyone look
through this example to learn about the transactions that are built on
it many other examples.

This example uses the C++ language bindings for libpmemobj, but extensive
knowledge of C++ is not required to understand it.

This is example consists of these files:

queue.cpp -- simple C++ program using libpmemobj transactions
Makefile  -- rules for building this example
run.sh    -- one way to run this example to illustrate what it does

To build this example run: make
To run it and see what it illustrates run: ./run.sh

Modifying the code and run steps is a great way to learn from this example.

This example shows a persistent linked list, implemented in C++
using libpmemobj.  Each time you run it, another element is added
to the persistent linked list, and the entire list is printed out.
If the program or machine crashes in the middle of updating the list,
incomplete updates are rolled back automatically by the library.
