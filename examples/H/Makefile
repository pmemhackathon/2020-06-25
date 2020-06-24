#
# Makefile for C++ find_bugs example
#
OBJS = find_bugs.o
CXXFLAGS = -g
LIBS = -lpmemobj -lpmem

all: find_bugs

find_bugs: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	$(RM) *.o core a.out

clobber: clean
	$(RM) find_bugs

.PHONY: all clean clobber
