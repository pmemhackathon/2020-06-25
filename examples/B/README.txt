The libpmemkv Key-Value Store

This example shows a high level API provided in multiple languages by
libpmemkv.  This example shows the concept of a persistent key-value
store that is built on top of lower-level libraries.  Reading through
this example is recommended for everyone since it provides an overview
of how the most common libraries are used together.

This example uses the C++ language bindings for libpmemkv, but you
don't need to know C++ to understand it.

This is example consists of these files:

pmemkv.cpp   -- simple C++ program using libpmemkv
kvinit.cpp   -- convenient function for creating/opening the kv store
Makefile     -- rules for building this example
run.sh       -- one way to run this example to illustrate what it does

To build this example run: make
To run it and see what it illustrates run: ./run.sh

Modifying the code and run steps is a great way to learn from this example.

This example shows a generic key-value store, called libpmemkv, which
handles all the details of persistent memory for you.  Each "put"
operation is immediately persistent, and transactional, maintaining
a consistent key-value store even if the program or machine crashes
unexpectedly.  This high-level API handles all the details like memory
mapping, allocation, flushing, etc. so the programmer doesn't have to
worry about them.

This example uses:
	- libpmemkv C++ bindings and "cmap" engine, which uses:
	- the "cmap" persistent memory concurrent hashmap, which uses:
	- libpmemobj for allocation & transactions, which uses:
	- libpmem for low-level mapping and flushing, which uses:
	- a DAX-mounted file system to get direct access to pmem

Although the above stack seems like lots of SW, it is all designed to
give applications direct access to their data where sits in pmem, rather
than copying data between buffers at each level.  So the operations you
see in the example, such as the "kvprint" function, are accessing the
persistent memory directly in persistent memory, not a copy fetched from
the key-value store each time.

Now that you've seen a simple, high-level API for using persistent memory,
continue on to the next example to see how you can build more complex
persistent memory programs using the libraries.
