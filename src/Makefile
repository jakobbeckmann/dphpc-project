CXX = g++
CXXFLAGS = -std=c++11 -Wall
 
EXEC = AlgorithmsRunner
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)

all: AlgorithmsRunner

AlgorithmsRunner.o: AlgorithmsRunner.cpp ChanAlgorithm.h
ChanAlgorithm.o: ChanAlgorithm.h ChanAlgorithm.cpp
AlgorithmsRunner: AlgorithmsRunner.o
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)