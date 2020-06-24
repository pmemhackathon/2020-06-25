#
# Makefile for basic_mmap example
#
PROGS = basic_mmap
CFLAGS = -g -Wall -Werror -std=gnu99

all: $(PROGS)

basic_mmap: basic_mmap.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

clean:
	$(RM) *.o a.out core

clobber: clean
	$(RM) $(PROGS) basic_mmap

.PHONY: all clean clobber
