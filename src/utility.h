/**
 * Some utility functionality for the Point class.
 */

#include <vector>

#include "Point.h"

int getOrientation(const Point& p1, const Point& p2, const Point& p3);
double getDistance(const Point& p1, const Point& p2);
size_t findLowestLeftmostPointIndex(const std::vector<Point>& points);
std::pair<int, int> findLowestPoint(const std::vector<std::vector<Point>>& hulls);
std::vector<Point> SplitVector(const std::vector<Point>& vec, size_t i, size_t n);
std::vector<Point> createPoints(int count, double min, double max);
std::vector<Point> readPointsFromFile(const std::string& filePath, size_t numPoints);

std::vector<Point> MergeVectors(std::vector<std::vector<Point>>&& hulls);
