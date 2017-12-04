CXX = mpic++
CXXFLAGS = -g -openmp

EXEC = Mpi_Algo
SOURCES = $(wildcard *.cpp)
SOURCES := $(filter-out main.cpp, $(SOURCES))
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)

Mpi_Algo: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS) $(OBJECTS)