CXX = mpicc
CXXFLAGS = -g -openmp

SOURCES = $(wildcard *.cpp)
SIMPLE_SOURCES := $(filter-out main.cpp Bin_Mpi_Algo.cpp, $(SOURCES))
BIN_SOURCES := $(filter-out main.cpp Simple_Mpi_Algo.cpp, $(SOURCES))
SIMPLE_OBJECTS = $(SIMPLE_SOURCES:.cpp=.o)
BIN_OBJECTS = $(BIN_SOURCES:.cpp=.o)

all: Simple_Mpi_Algo Bin_Mpi_Algo

Simple_Mpi_Algo: $(SIMPLE_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o Simple_Mpi_Algo

Bin_Mpi_Algo: $(BIN_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o Bin_Mpi_Algo

clean:
	rm -f $(EXEC) $(SIMPLE_OBJECTS) $(BIN_OBJECTS)