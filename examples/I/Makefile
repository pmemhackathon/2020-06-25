#
# Makefile for finding words examples
#
OBJS = find_words.o
LIBS = -lpmemobj -lpmem

all: find_words

find_words: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	$(RM) *.o core a.out

clobber: clean
	$(RM) find_words

.PHONY: all clean clobber

