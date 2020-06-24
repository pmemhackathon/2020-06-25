#
# Makefile for volatile example
#
PROGS = volatile
CFLAGS = -g -Wall -Werror -std=gnu99
LIBS = -lmemkind

all: $(PROGS)

volatile: volatile.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

clean:
	$(RM) *.o a.out core

clobber: clean
	$(RM) $(PROGS) volatile

.PHONY: all clean clobber
