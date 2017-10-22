/**
 * Some utility functionality for the Point class.
 */

#include <vector>

#include "Point.h"

int getOrientation(Point p1, Point p2, Point p3);
double getDistance(Point p1, Point p2);
int lowestAngleSort(const void *vpp1, const void *vpp2);
std::vector<Point> createPoints(int count, double min, double max);
void writePointsToFile(std::vector<Point>, std::string fileName);
