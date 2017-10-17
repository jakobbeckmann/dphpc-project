/**
    Contains some utility functions for the Points class.
    TODO: In fact I think it would be nicer to have those functions integrated in the actual Point class...
 */

#include <cmath>
#include <random>
#include <fstream>

#include "utility.h"


/**
    Returns the orientation of the angle between three points.
    Returns -1: Right turn
             1: Left turn
             0: Collinear
    @param p1: first point
    @param p2: second point
    @param p3: third point
*/
int orientation(Point p1, Point p2, Point p3) {
    double cross_product = (p1.y - p2.y) * (p3.x - p2.x) - (p1.x - p2.x) * (p3.y - p2.y);
    if (fabs(cross_product) < EPSILON)
        return COLLINEAR;
    return (cross_product > 0)? LEFT_TURN: RIGHT_TURN;
}

/**
    Returns square of the distance between two 2D points.
    @param point1: first point.
    @param point2: second point.
*/
double distance(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

/**
    Function used when sorting using qsort().
    Returns the point with lowest polar angle w.r.t p0(0, 0) and the x-axis.
    @param vpp1: pointer to first point
    @param vpp2: pointer to second point
*/
int compare(const void* vpp1, const void* vpp2) {
    Point p0 = Point();
    Point* pp1 = (Point*) vpp1;
    Point* pp2 = (Point*) vpp2;
    int orient = orientation(p0, *pp1, *pp2);
    if(orient == COLLINEAR) {
        return (distance(p0, *pp1) <= distance(p0, *pp2))? 1: -1;
    }
    return (orient == RIGHT_TURN)? 1: -1;
}

std::vector<Point> createPoints(double pointCount) {
    std::vector<Point> points;

    for(int idx = 0; idx < pointCount; idx++) {
        double x = (double) rand() / (RAND_MAX);
        double y = (double) rand() / (RAND_MAX);
        points.emplace_back(Point(x, y));
    }
    return points;

}

void pointsVectorToFile(std::vector<Point> pointVector, std::string fileName) {
    std::ofstream allPointsFile;
    allPointsFile.open(fileName);
    for (Point point : pointVector) {
        allPointsFile << point.x << "," << point.y << std::endl;
    }
    allPointsFile.close();
}