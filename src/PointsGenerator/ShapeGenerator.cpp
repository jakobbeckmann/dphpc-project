//
// Created by Polly Lilly on 19.11.17.
//

#include <cmath>
#include <random>
#include <fstream>

#include "ShapeGenerator.h"

/**
 * Generates random points for given range in random shape;
 * @param count number of random points
 * @param range +range for maximum possible value of x/y coordinate and -range for minimum
 * @return
 */
std::vector<Point> randomShapeGenerator(int count, double range) {
    double min = -range;
    double max = range;
    size_t nSubSets = 5;

    std::vector<Point> finalPoints;

    for (size_t i = 0; i < nSubSets + 1; i++) {
        std::vector<Point> points;
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

        std::uniform_real_distribution<> dis(min, max);

        size_t nPointsInSubSet = (i < nSubSets)? count / nSubSets : count % nSubSets;

        for(size_t idx = 0; idx < nPointsInSubSet; idx++) {
            auto x = (double) dis(gen);
            auto y = (double) dis(gen);
            double xOffset = ((double) std::rand() / (RAND_MAX)) * 20 - 5;
            double yOffset = ((double) std::rand() / (RAND_MAX)) * 20 - 5;
            points.emplace_back(Point(x + xOffset, y + yOffset));
        }
        finalPoints.insert(finalPoints.end(), points.begin(), points.end());
    }
    return finalPoints;
}

/**
 * Generates random points for given range in circle shape;
 * @param count number of random points
 * @param range +range for maximum possible value of x/y coordinate and -range for minimum
 * @return
 */
std::vector<Point> circleShapeGenerator(int count, double range) {
    double min = -range;
    double max = range;
    size_t nSubSets = 5;
    std::vector<Point> finalPoints;
    return finalPoints;
}

/**
 * Generates random points for given range in square shape;
 * @param count number of random points
 * @param range +range for maximum possible value of x/y coordinate and -range for minimum
 * @return
 */
std::vector<Point> squareShapeGenerator(int count, double range) {
    double min = -range;
    double max = range;
    size_t nSubSets = 5;
    std::vector<Point> finalPoints;
    return finalPoints;
}