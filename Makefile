# Very simple makefile which assumes one program per cpp source file.
SOURCES := $(wildcard *.cpp) # get list of cpp files
TARGETS := $(SOURCES:.cpp=.o)  # strip .cpp extension for list of programs

CXXFLAGS := -O2 -std=c++11 -Wall -Wformat=2 -march=native -DNDEBUG
CC       := g++
LDADD    := -lntl -lgmp -lm

# {{{ for debugging
# DBGFLAGS := -g -O0 -UNDEBUG
# debug : CXXFLAGS += $(DBGFLAGS)
# debug : all
# .PHONY : debug
# }}}

main: Party.o Member.o Function.o main.o
	$(CC) main.o Function.o Member.o Party.o $(CXXFLAGS) -o main $(LDADD)
	
main.o: main.cpp gmp_defs.h
	$(CC) -std=c++11 -c main.cpp -o main.o

Function.o: Function.cpp Function.h gmp_defs.h
	$(CC) -std=c++11 -c Function.cpp -o Function.o
	
Member.o: Member.cpp Member.h gmp_defs.h
	$(CC) -std=c++11 -c Member.cpp -o Member.o
	
Party.o: Party.cpp Party.h gmp_defs.h
	$(CC) -std=c++11 -c Party.cpp -o Party.o

#for each  source file "foo.cpp", compile into program named "foo"
# $(TARGETS) : % : %.cpp
# 	$(CC) $(CXXFLAGS) -o $@ $< $(LDADD)

.PHONY : clean
clean :
	rm -f $(TARGETS)

# vim:ft=make:foldmethod=marker:foldmarker={{{,}}}
