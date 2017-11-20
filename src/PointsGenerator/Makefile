CXX = g++
CXXFLAGS = -std=c++11 -Wall
 
EXEC = PointsGenerator
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: PointsGenerator

PointsGenerator.o: PointsGenerator.cpp
ShapeGenerator.o: ShapeGenerator.h ShapeGenerator.cpp
PointsGenerator: PointsGenerator.o ShapeGenerator.o
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)