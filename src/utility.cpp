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
int findLowestLeftmostPointIndex(std::vector<Point> &points) {
    int result = 0;
    double lowest = points[0].y;
    for (int idx = 1; idx < points.size(); idx++) {
        if (points[idx].y < lowest || (points[idx].y == lowest && points[idx].x < points[result].x)) {
            lowest = points[idx].y;
            result = idx;
        }
    }

    std::cout << "Found lowest left index: " << result << std::endl;
    std::cout << "Value: " << points[result].x << " " << points[result].y << std::endl;
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
    std::vector<Point> points;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(min, max);
    for(int idx = 0; idx < count; idx++) {
        double x = (double) dis(gen);
        double y = (double) dis(gen);
        points.emplace_back(Point(x, y));
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
    for (int hull_idx = 0; hull_idx < hulls.size(); hull_idx++) {
        for (int point_idx = 0; point_idx < hulls[hull_idx].size(); point_idx++) {
            if (hulls[hull_idx][point_idx].y < lowest_y) {
                lowest_y = hulls[hull_idx][point_idx].y;
                hull = hull_idx;
                point = point_idx;
            }
        }
    }
    return std::make_pair(hull, point);
}

std::vector<std::vector<Point>> SplitVector(const std::vector<Point>& vec, size_t n)
{
    std::vector<std::vector<Point>> outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    std::cout << "sub vectors contain " << length << " elements with remainder " << remain << std::endl;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(n, vec.size()); ++i)
    {
        end += (remain > 0) ? (length + !!(remain--)) : length;

        outVec.emplace_back(std::vector<Point>(vec.begin() + begin, vec.begin() + end));

        begin = end;
    }

    return outVec;
}
