#
# Makefile for lowlevel example
#
PROGS = lowlevel
CFLAGS = -g -Wall -Werror -std=gnu99
LIBS = -lpmem

all: $(PROGS)

lowlevel: lowlevel.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

clean:
	$(RM) *.o a.out core

clobber: clean
	$(RM) $(PROGS) lowlevel

.PHONY: all clean clobber
