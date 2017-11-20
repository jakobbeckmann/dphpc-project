//
// Created by Polly Lilly on 19.11.17.
//

#include <cstdio>
#include <iostream>
#include <vector>

#include "../Point.h"
#include "ShapeGenerator.h"

using namespace std;

int main(int argc, char const *argv[]) {
    int numberOfPoints = atoi(argv[1]);
    int range = atoi(argv[2]);
    string shape = argv[3];

    std::vector<Point> points;

    if (shape == "random") {
        points = randomShapeGenerator(numberOfPoints, range);
    } else if (shape == "circle") {
        points = circleShapeGenerator(numberOfPoints, range);
    } else if (shape == "disk") {
        points = diskShapeGenerator(numberOfPoints, range);
    } else if (shape == "square") {
        points = squareShapeGenerator(numberOfPoints, range);
    } else {
        std::cout << "No such shape!";
        std::exit(EXIT_FAILURE);
    }

    for (vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        it->print();
    }

    return 0;
}

