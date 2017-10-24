/**
    Contains some utility functions for the Points class.
    TODO: In fact I think it would be nicer to have those functions integrated in the actual Point class...
 */

#include <cmath>
#include <random>
#include <fstream>
#include <iostream>

#include "utility.h"


/**
    Returns the orientation of the angle between three points.
    Returns -1: Anticlockwise
             1: Clockwise
             0: Collinear
    @param p1: first point
    @param p2: second point
    @param p3: third point
*/
int getOrientation(Point p1, Point p2, Point p3) {
    double cross_product = (p2.y - p1.y) * (p3.x - p2.x) - (p3.y - p2.y) * (p2.x - p1.x);
    if (fabs(cross_product) < EPSILON)
        return COLLINEAR;
    return (cross_product > 0)? CLOCKWISE: ANTICLOCKWISE;
}

/**
    Returns square of the distance between two 2D points.
    @param point1: first point.
    @param point2: second point.
*/
double getDistance(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

/**
 * Generates random points for given min and max x/y coordinates.
 * @param count number of random points
 * @param min minimum possible value of x/y coordinate
 * @param max maximum possible value of x/y coordinate
 * @return
 */
std::vector<Point> createPoints(int count, double min, double max) {
    std::vector<Point> points;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(min, max);
    for(int idx = 0; idx < count; idx++) {
        double x = (double) dis(gen);
        double y = (double) dis(gen);
        points.emplace_back(Point(x, y));
    }

    /*points.emplace_back(Point(1, 1));
    points.emplace_back(Point(1, 3));
    points.emplace_back(Point(3, 2));
    points.emplace_back(Point(3, 5));
    points.emplace_back(Point(5, 3));
    points.emplace_back(Point(5, 1));
    points.emplace_back(Point(6, 1));
    points.emplace_back(Point(2.5, 3.95));*/

    return points;

}

/**
 * Writes given set of points to a file with a given filename.
 * @param points
 * @param fileName
 */
void writePointsToFile(const std::vector<Point>& points, const std::string& fileName) {
    std::ofstream allPointsFile;
    allPointsFile.open(fileName);
    for (Point point : points) {
        allPointsFile << point.x << "," << point.y << std::endl;
    }
    allPointsFile.close();
}
