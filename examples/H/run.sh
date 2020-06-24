#!/bin/bash -ex
#
# shell commands to run find_bugs example
#

# create the pmem pool if it doesn't already exist
if [ ! -f /pmem/find_bugs ]
then
	pmempool create obj --layout=find_bugs -s 20M /pmem/find_bugs
fi

# run the program under pmemcheck
valgrind --tool=pmemcheck ./find_bugs /pmem/find_bugs
