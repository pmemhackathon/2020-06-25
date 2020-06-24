#!/bin/bash -ex
#
# shell commands to run freq_pmem_cpp example
#

# create the pmem pool if it doesn't already exist
if [ ! -f /pmem/freqcountcpp ]
then
	pmempool create obj --layout=freq -s 10M /pmem/freqcountcpp
fi

# run the frequency count on three files concurrently
./freq_pmem_cpp /pmem/freqcountcpp words.txt words.txt words.txt

# show the persistent result
./freq_pmem_print /pmem/freqcountcpp

# count some more words
./freq_pmem_cpp /pmem/freqcountcpp words.txt words.txt words.txt

# show the persistent result
./freq_pmem_print /pmem/freqcountcpp
