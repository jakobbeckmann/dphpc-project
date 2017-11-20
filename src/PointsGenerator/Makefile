CXX = g++
CXXFLAGS = -std=c++11 -Wall
 
EXEC = PointsGenerator
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)

all: PointsGenerator

PointsGenerator.o: PointsGenerator.cpp ShapeGenerator.h
ShapeGenerator.o: ShapeGenerator.h ShapeGenerator.cpp
PointsGenerator: PointsGenerator.o
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)