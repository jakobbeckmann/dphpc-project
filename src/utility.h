/**
 * Some utility functionality for the Point class.
 */

#include <vector>

#include "Point.h"

int orientation(Point p1, Point p2, Point p3);
double dist_square(Point p1, Point p2);
int check_orientation(const void *vpp1, const void *vpp2);
std::vector<Point> createPoints(double pointCount);
void pointsVectorToFile(std::vector<Point>, std::string fileName);
