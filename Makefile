CXX=g++
CXXFLAGS=-std=c++11 -Wall -I$(CURDIR) -fopenmp
LDFLAGS=-fopenmp
SRCS=main.cc color.cc graph/adj_matrix_graph.cc graph/io.cc
OBJS=$(subst .cc,.o,$(SRCS))
DEPS=color.h graph/adj_matrix_graph.h graph/io.h
RM=rm -f

all: run

run: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	$(RM) $(OBJS)
