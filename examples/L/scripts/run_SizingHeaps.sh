#!/bin/bash -ex
#
# shell commands to run the SizingHeaps example
#

# create the llpl directory on the pmem file system
[ -d /pmem/llpl ] || mkdir /pmem/llpl

# remove any leftovers from previous runs
rm -rf /pmem/llpl/{fixed,growable,growable_with_limit}

# the first run will create the pmem file
./scripts/run workshop.Example02_SizingHeaps
