#!/bin/bash -ex
#
# shell commands to run this example
#

# run the example program on pmem
./lowlevel /pmem/rawfile

# dump the file to show contents
hexdump -C /pmem/rawfile
