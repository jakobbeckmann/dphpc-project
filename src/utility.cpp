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
 * Generates random points for given min and max x/y coordinates.
 * @param count number of random points
 * @param min minimum possible value of x/y coordinate
 * @param max maximum possible value of x/y coordinate
 * @return
 */

/**
    Function used when sorting using qsort().
    Returns the point with lowest polar angle w.r.t p0(0, 0) and the x-axis.
    @param vpp1: pointer to first point
    @param vpp2: pointer to second point
*/
/*
int lowestAngleSort(const void *vpp1, const void *vpp2) {
    Point p0;
    Point* pp1 = (Point*) vpp1;
    Point* pp2 = (Point*) vpp2;
    int orient = getOrientation(p0, *pp1, *pp2);
    if(orient == COLLINEAR) {
        return (getDistance(p0, *pp1) <= getDistance(p0, *pp2))? -1: 1;
    }
    return (orient == CLOCKWISE)? 1: -1;
}
*/

/**
 *  Swap two points with idx1 and idx2 in a points vector.
 */
void swapPoints(vector<Point> &points, int idx1, int idx2) {
    Point p = points[idx2];
    points[idx2] = points[idx1];
    points[idx1] = p;
}

/**
 * Finds the index of the most bottom left point. Used to start the graham scan.
 */
int findLowestLeftmostPointIndex(vector<Point> &points) {
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
