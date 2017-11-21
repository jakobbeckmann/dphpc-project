# MacOS is special :)
OS := $(shell uname)
ifeq ($(OS), Darwin)
# Mac OS X
CXX = g++-7
else
CXX = g++
endif
CXXFLAGS = -std=c++11 -Wall -fopenmp -O2 -w
 
EXEC = AlgorithmsRunner
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: AlgorithmsRunner

AlgorithmsRunner.o: AlgorithmsRunner.cpp
ChanAlgorithm.o: ChanAlgorithm.cpp ChanAlgorithm.h
GrahamScanAlgorithm.o: GrahamScanAlgorithm.cpp GrahamScanAlgorithm.h
utility.o: utility.cpp utility.h
AlgorithmsRunner: AlgorithmsRunner.o ChanAlgorithm.o GrahamScanAlgorithm.o utility.o
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)