#!/bin/bash -ex
#
# shell commands to run find_words example
#

# show the contents of the imput data set
echo "dictionary:"
cat dictionary.txt
echo "\n\nboard:"
cat board.txt
echo "\n"

# run the find_words program with given data set
./find_words /pmem/find_words solve dictionary.txt board.txt

# print found words
./find_words /pmem/find_words print
