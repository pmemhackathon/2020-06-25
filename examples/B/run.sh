#!/bin/bash -ex
#
# shell commands to run this example
#

# add some values
./pmemkv /pmem/kvfile bach 1685-1750
./pmemkv /pmem/kvfile mozart 1756-1791

# print a value
./pmemkv /pmem/kvfile bach

# add some more
./pmemkv /pmem/kvfile beethoven 1770-1827
./pmemkv /pmem/kvfile brahms 1833-1897
./pmemkv /pmem/kvfile haydn 1732-1809

# print all k-v pairs in kvfile
./pmemkv /pmem/kvfile
