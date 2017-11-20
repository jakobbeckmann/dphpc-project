//
// Created by Polly Lilly on 19.11.17.
//
#include <cstdio>
#include <iostream>
#include <vector>

#include "Point.h"
#include "ChanAlgorithm.h"

using namespace std;

int main(int argc, char const *argv[]) {
    int numberOfCores = atoi(argv[1]);
    int numberOfPoints = atoi(argv[2]);
    string algorithm = argv[3];

    std::vector<Point> points;

    for (int idx = 0; idx < numberOfPoints; idx++) {
        double x, y;
        cin >> x >> y;
        points.push_back(Point(x, y));
    }

    std::vector<Point> result;
    double totalTime = 0;

    if (algorithm == "version1") {
        ChanAlgorithm chan;
        // measure time
        result = chan.run(points, numberOfCores);
        totalTime = 10;
    } else {
        std::cout << "No such algorithm!";
        std::exit(EXIT_FAILURE);
    }

    std::cout << totalTime << endl;
    for (vector<Point>::iterator it = result.begin(); it != result.end(); ++it) {
        it->print();
    }

    return 0;
}

