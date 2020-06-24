#!/bin/bash -ex
#
# shell commands to run the PerformanceIdioms example
#

# create the llpl directory on the pmem file system
[ -d /pmem/llpl ] || mkdir /pmem/llpl

# remove any leftovers from previous runs
rm -f /pmem/llpl/{tx_,}performance_idioms

./scripts/run workshop.Example05_PerformanceIdioms
