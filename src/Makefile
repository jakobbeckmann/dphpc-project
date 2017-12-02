CXX = mpicc
CXXFLAGS = -g -openmp

EXEC = Mhull
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: Mhull

Mhull.o: Mhull.cpp
Graham_Jarvis.o: Graham_Jarvis.cpp Graham_Jarvis.hpp
JarvisAlgorithm.o: JarvisAlgorithm.cpp JarvisAlgorithm.h
GrahamScanAlgorithm.o: GrahamScanAlgorithm.cpp GrahamScanAlgorithm.h
utility.o: utility.cpp utility.h
Mhull: Mhull.o Graham_Jarvis.o Graham_Jarvis.o JarvisAlgorithm.o GrahamScanAlgorithm.o utility.o
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)
