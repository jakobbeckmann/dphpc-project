CXX = mpic++
CXXFLAGS = -g -fopenmp -std=c++11

EXEC = Mpi_Algo
SOURCES = $(wildcard *.cpp)
SOURCES := $(filter-out main.cpp, $(SOURCES))
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)

Mpi_Algo: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS) $(OBJECTS)