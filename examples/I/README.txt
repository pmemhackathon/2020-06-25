C++ data structures in persistent memory

In this example, we will solve a simple algorithmic problem using data
structures which resides in persistent memory. We will use persistent containers
from libpmemobj-cpp library.

C++ programmers will get the most out of this example.

This is example consists of these files:

dictionary.txt       -- a file with some words which represents a dictionary
board.txt            -- a file with characters in a form of 2D matrix
Makefile             -- rules for building all the programs in this example
trie.hpp             -- a simple trie structure implementation for persistent
                        memory
find_words.cpp       -- C++ program for finding words, program can continue
                        search process after the crash happen
run.sh               -- run the find_words example

To build the program in this example run: make
To run it, use the run.sh script.

Modifying the code and run steps is a great way to learn from this example.

In this example we are given a problem to solve:
In a 2-D matrix of characters (board.txt) we must find all the words from a
dictionary (dictionary.txt). Words can be formed from the adjecent cells (left,
right, top, bottom), each cell can be used only once when creating a single
word.

We will solve this problem, creating trie and performing DFS from every board
cell, saving current results in on a persistent memory.
