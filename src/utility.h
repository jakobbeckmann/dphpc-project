/**
 * Some utility functionality for the Point class.
 */

#include <vector>

#include "Point.h"

using namespace std;

int getOrientation(const Point& p1, const Point& p2, const Point& p3);
double getDistance(const Point& p1, const Point& p2);
std::vector<Point> createPoints(int count, double min, double max);
void swapPoints(vector<Point> &points, int indx1, int indx2);
int findLowestLeftmostPointIndex(vector<Point> &points);