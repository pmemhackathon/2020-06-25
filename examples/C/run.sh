#!/bin/bash -ex
#
# shell commands to run this example
#

# add some values to the queue
./queue /pmem/qfile push 200
./queue /pmem/qfile push 400
./queue /pmem/qfile push 600

# print the queue
./queue /pmem/qfile show

# pop a couple values
./queue /pmem/qfile pop
./queue /pmem/qfile pop

# add another one
./queue /pmem/qfile push 800

# print the queue
./queue /pmem/qfile show
