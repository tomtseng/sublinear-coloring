CXX=g++
CXXFLAGS=-std=c++11 -mavx -Wall
LDFLAGS=
SRCS=main.cc
OBJS=$(subst .cc,.o,$(SRCS))
RM=rm -f

all: main

main: $(OBJS)
	$(CXX) $(LDFLAGS) -o main main.o

main.o: main.cc
	$(CXX) $(CPPFLAGS) -c main.cc

clean:
	$(RM) $(OBJS)
