#
# Makefile for simple pmemkv C++ example
#
OBJS = pmemkv.o kvinit.o
CXXFLAGS = -std=c++17
LIBS = -lpmemkv

all: pmemkv

pmemkv: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	$(RM) *.o core a.out

clobber: clean
	$(RM) pmemkv

.PHONY: all clean clobber
