#pragma once

#include "FileWriter.h"
#include "utility.h"

class ChanAlgorithm {
public:
    static int findTangentIndex(const std::vector<Point>& points, Point base);
    static std::pair<int, int> findNextMergePoint(const std::vector<std::vector<Point>>& hulls, std::pair<int, int> base_pair);

    static std::vector<Point> mergeAllHulls(const std::vector<std::vector<Point>>& hulls);
    static std::vector<Point> mergeTwoHulls(std::vector<Point>&& a, std::vector<Point>&& b);

    static std::vector<Point> run(const std::vector<Point>& points, int parallel_idx, size_t parts);

};

class ChanAlgorithm2Merge : private ChanAlgorithm {
public:
    static std::vector<Point> run(const std::vector<Point>& points, int parallel_idx, size_t parts);
};
