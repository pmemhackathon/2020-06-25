#!/bin/bash -ex
#
# shell commands to run freq example
#

# show thew contents of our small data set
cat words.txt

# run the volatile example, just to show it does indeed count the words
./freq -p words.txt
