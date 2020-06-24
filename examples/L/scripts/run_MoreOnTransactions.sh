#!/bin/bash -ex
#
# shell commands to run the MoreOnTransactions example
#

# create the llpl directory on the pmem file system
[ -d /pmem/llpl ] || mkdir /pmem/llpl

# remove any leftovers from previous runs
rm -f /pmem/llpl/more_on_transactions

./scripts/run workshop.Example04_MoreOnTransactions
