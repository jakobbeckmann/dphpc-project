/**
    Contains some utility functions for the Points class.
    TODO: In fact I think it would be nicer to have those functions integrated in the actual Point class...
 */

#include <cmath>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <string>

#include "utility.h"

#ifndef WRITE_DEBUG
#define WRITE_DEBUG 0
#endif

/**
    Returns the orientation of the angle between three points.
    Returns -1: Anticlockwise
             1: Clockwise
             0: Collinear
    @param p1: first point (origin for both vectors going to p2 and p3)
    @param p2: second point
    @param p3: third point
*/
int getOrientation(const Point& p1, const Point& p2, const Point& p3) {
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
double getDistance(const Point& p1, const Point& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

/**
 * Finds the index of the most bottom left point. Used to start the graham scan.
 */
size_t findLowestLeftmostPointIndex(const std::vector<Point>& points) {
    size_t result = 0;
    double lowest = points[0].y;
    for (size_t idx = 1; idx < points.size(); idx++) {
        if (points[idx].y < lowest || (points[idx].y == lowest && points[idx].x < points[result].x)) {
            lowest = points[idx].y;
            result = idx;
        }
    }

    return result;
}

/**
 * Generates random points for given min and max x/y coordinates.
 * @param count number of random points
 * @param min minimum possible value of x/y coordinate
 * @param max maximum possible value of x/y coordinate
 * @return
 */
std::vector<Point> createPoints(int count, double min, double max) {

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

std::vector<Point> readPointsFromFile(const std::string& filePath, size_t numPoints)
{
    std::vector<Point> points;
	points.reserve(numPoints);

    std::ifstream source(filePath);
    std::string line;

    while (std::getline(source, line)) {
        double x, y;
        std::istringstream ss(line);
        ss >> x >> y;
        //std::cout << x << ", " << y << "\n";
        Point point(x,y);
        points.push_back(point);
    }
    return points;
}


/**
    Returns a std::pair representing the hull number and point index inside that hull. The represented
    point is the point with lowest y coordinate across all hulls.
    @param hulls: std::vector of hulls (vectors of points)
*/
std::pair<int, int> findLowestPoint(const std::vector<std::vector<Point>>& hulls) {
    int hull = 0, point = 0;
    double lowest_y = hulls[0][0].y;
    for (size_t hull_idx = 0; hull_idx < hulls.size(); hull_idx++) {
        for (size_t point_idx = 0; point_idx < hulls[hull_idx].size(); point_idx++) {
            if (hulls[hull_idx][point_idx].y < lowest_y) {
                lowest_y = hulls[hull_idx][point_idx].y;
                hull = hull_idx;
                point = point_idx;
            }
        }
    }
    return std::make_pair(hull, point);
}

std::vector<Point> SplitVector(const std::vector<Point>& vec, size_t i, size_t n)
{
    size_t length = vec.size() / n;

#if WRITE_DEBUG
    size_t remain = vec.size() % n;
    if (i == 0)
        std::cout << "sub vectors contain " << length << " elements with remainder " << remain << std::endl;
#endif

    size_t begin = i*length;
    size_t end = i+1 == n ? vec.size() : std::min((i+1)*length, vec.size());

    assert(begin < end);

    return std::vector<Point>(vec.begin() + begin, vec.begin() + end);
}

/**
 * Merges all the vectors into one vector. The original vectors are moved from and left in an unspecified state.
 */
std::vector<Point> MergeVectors(std::vector<std::vector<Point>>&& hulls)
{
    std::vector<Point> hull_points;
    size_t sum_sizes = 0;
    for (const std::vector<Point>& hull : hulls)
        sum_sizes += hull.size();

    hull_points.reserve(sum_sizes);

    for (std::vector<Point>& hull : hulls)
        std::move(hull.begin(), hull.end(), std::back_inserter(hull_points));

    return hull_points;
}
