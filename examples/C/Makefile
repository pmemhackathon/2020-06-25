#
# Makefile for C++ queue example
#
OBJS = queue.o
LIBS = -lpmemobj -lpmem

all: queue

queue: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	$(RM) *.o core a.out

clobber: clean
	$(RM) queue

.PHONY: all clean clobber
