#pragma once

#include "FileWriter.h"
#include "utility.h"

class ChanAlgorithm {
private:
    Point startingPoint;

    int lowestAngleSort(const Point& pp1, const Point& pp2);
    int findTangentIndex(const std::vector<Point>& points, Point base);
    std::vector<Point> grahamScan(std::vector<Point>& points, int subsetIdx);
    std::pair<int, int> findLowestPoint(const std::vector<std::vector<Point>>& hulls);
    std::pair<int, int> findNextMergePoint(const std::vector<std::vector<Point>>& hulls, std::pair<int, int> base_pair);

public:
    std::vector<Point> run(const std::vector<Point>& points, size_t parallel_idx);

};
