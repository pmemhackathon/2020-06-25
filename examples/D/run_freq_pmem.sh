#!/bin/bash -ex
#
# shell commands to run freq_pmem example
#

# create the pmem pool if it doesn't already exist
if [ ! -f /pmem/freqcount ]
then
	pmempool create obj --layout=freq -s 10M /pmem/freqcount
fi

# show there's no data in the pool initially
./freq_pmem_print /pmem/freqcount

# run the frequency count on three files concurrently
./freq_pmem /pmem/freqcount words.txt words.txt words.txt

# show the persistent result
./freq_pmem_print /pmem/freqcount

# count some more words
./freq_pmem /pmem/freqcount words.txt words.txt words.txt

# show that the results are accumulating in the persistent data structure
./freq_pmem_print /pmem/freqcount
