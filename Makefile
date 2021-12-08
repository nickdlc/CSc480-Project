# Very simple makefile which assumes one program per cpp source file.
SOURCES := $(wildcard *.cpp) # get list of cpp files
TARGETS := $(SOURCES:.cpp=)  # strip .cpp extension for list of programs

CXXFLAGS := -O2 -std=c++11 -Wall -Wformat=2 -march=native -DNDEBUG
CC       := g++
LDADD    := -lntl -lgmp -lm

.PHONY : all
all : $(TARGETS)

# {{{ for debugging
# DBGFLAGS := -g -O0 -UNDEBUG
# debug : CXXFLAGS += $(DBGFLAGS)
# debug : all
# .PHONY : debug
# }}}

# for each source file "foo.cpp", compile into program named "foo"
$(TARGETS) : % : %.cpp
	$(CC) $(CXXFLAGS) -o $@ $< $(LDADD)

.PHONY : clean
clean :
	rm -f $(TARGETS)

# vim:ft=make:foldmethod=marker:foldmarker={{{,}}}
