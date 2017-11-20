CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
 
EXEC = AlgorithmsRunner
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: AlgorithmsRunner

AlgorithmsRunner.o: AlgorithmsRunner.cpp
ChanAlgorithm.o: ChanAlgorithm.cpp ChanAlgorithm.h
utility.o: utility.cpp utility.h
AlgorithmsRunner: AlgorithmsRunner.o ChanAlgorithm.o utility.o
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)