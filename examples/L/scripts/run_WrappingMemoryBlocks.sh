#!/bin/bash -ex
#
# shell commands to run the WrappingMemoryBlocks example
#

# create the llpl directory on the pmem file system
[ -d /pmem/llpl ] || mkdir /pmem/llpl

# remove any leftovers from previous runs
rm -f /pmem/llpl/wrapping_memory_blocks

# first run (no pmem file has been created yet)
./scripts/run workshop.Example06_WrappingMemoryBlocks

# show the pmem file
ls -l /pmem/llpl/wrapping_memory_blocks

# second run (pmem file exists already)
./scripts/run workshop.Example06_WrappingMemoryBlocks

