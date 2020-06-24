#!/bin/bash -ex
#
# shell commands to build the GettingStarted example
#

javac -cp ./:target/classes -d target/classes workshop/Example01_GettingStarted.java
