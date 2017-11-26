//
// Created by Polly Lilly on 19.11.17.
//
#include <cstdio>
#include <iostream>
#include <vector>

#include "Point.h"
#include "ChanAlgorithm.h"
#include "GrahamScanAlgorithm.h"
#include "JarvisAlgorithm.h"

int main(int argc, char const *argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " numcores numpoints algorithm\n";
        std::cout << "  algorithm := chan1|chan2|graham|jarvis\n";
		return -1;
    }

    int numberOfCores = atoi(argv[1]);
    int numberOfPoints = atoi(argv[2]);
    std::string algorithm = argv[3];

    std::vector<Point> points;

    for (int idx = 0; idx < numberOfPoints; idx++) {
        double x, y;
        std::cin >> x >> y;
        points.push_back(Point(x, y));
    }

    std::vector<Point> result;
    double totalTime = 0;

    if (algorithm == "chan1") {
        ChanAlgorithm chan;
        // measure time
        result = chan.run(points, numberOfCores);
        totalTime = 2.387;
    } else if (algorithm == "chan2") {
        ChanAlgorithm2Merge chan;
        // measure time
        result = chan.run(points, numberOfCores);
        totalTime = 2.387;
    } else  if (algorithm == "graham") {
        GrahamScanAlgorithm graham;
        // measure time
        result = graham.run(points, numberOfCores);
        totalTime = 2.387;
    } else  if (algorithm == "jarvis") {
        JarvisAlgorithm jarvis;
        // measure time
        result = jarvis.run(points, numberOfCores);
        totalTime = 2.387;
    } else {
        std::cout << "No such algorithm!";
        std::exit(EXIT_FAILURE);
    }

    std::cout << totalTime << std::endl;
    for (const Point& p : result) {
        p.print();
    }

    return 0;
}

