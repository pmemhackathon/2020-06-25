#!/bin/bash -ex
#
# shell commands to run this example
#

# create a one megabyte pmem file full of zeros
rm -f /pmem/myfile
truncate --size=1M /pmem/myfile

# dump the file to show initial contents
hexdump -C /pmem/myfile

# run the example program
./basic_mmap /pmem/myfile

# dump the file to show new contents
hexdump -C /pmem/myfile
