#!/bin/bash -ex
#
# shell commands to run the UsingOtherHeaps example
#

# create the llpl directory on the pmem file system
[ -d /pmem/llpl ] || mkdir /pmem/llpl

# remove any leftovers from previous runs
rm -f /pmem/llpl/using_other_heaps_{transactional,persistent}

# first run (no pmem files have been created yet)
./scripts/run workshop.Example03_UsingOtherHeaps

# show the pmem files
ls -l /pmem/llpl/using_other_heaps*

# second run (pmem files exist already)
./scripts/run workshop.Example03_UsingOtherHeaps
