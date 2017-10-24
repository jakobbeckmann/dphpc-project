/**
 * Some utility functionality for the Point class.
 */

#include <vector>

#include "Point.h"

using namespace std;

int getOrientation(Point p1, Point p2, Point p3);
double getDistance(Point p1, Point p2);
int lowestAngleSort(const void *vpp1, const void *vpp2);
std::vector<Point> createPoints(int count, double min, double max);
void swapPoints(vector<Point> &points, int indx1, int indx2);
int findLowestLeftmostPointIndex(vector<Point> &points);