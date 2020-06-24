#!/bin/bash -ex
#
# shell commands to run the GettingStarted example
#

# create the llpl directory on the pmem file system
[ -d /pmem/llpl ] || mkdir /pmem/llpl

# remove any leftovers from previous runs
rm -f /pmem/llpl/getting_started

# first run (no pmem file has been created yet)
./scripts/run workshop.Example01_GettingStarted

# show the pmem file that was created
ls -l /pmem/llpl/getting_started

# second run (pmem file exists already)
./scripts/run workshop.Example01_GettingStarted
