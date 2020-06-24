#
# Makefile for word frequency count examples
#
PROGS = freq freq_mt freq_pmem freq_pmem_print
CFLAGS = -g -Wall -Werror -std=gnu99

all: $(PROGS)

freq_mt: LIBS = -pthread
freq_pmem freq_pmem_print freq_pmem_cpp: LIBS = -lpmem -lpmemobj -pthread

freq: freq.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

freq_mt: freq_mt.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

freq_pmem: freq_pmem.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

freq_pmem_print: freq_pmem_print.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

clean:
	$(RM) *.o a.out core

clobber: clean
	$(RM) $(PROGS)

.PHONY: all clean clobber
