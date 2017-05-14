CXX=g++
CXXFLAGS=-std=c++11 -Wall -I$(CURDIR) -mavx -fopenmp
LDFLAGS=-fopenmp
SRCS=main.cc graph/adj_matrix_graph.cc graph/io.cc
OBJS=$(subst .cc,.o,$(SRCS))
DEPS=hellomake.h
RM=rm -f

all: run

run: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(Cxx) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	$(RM) $(OBJS)
