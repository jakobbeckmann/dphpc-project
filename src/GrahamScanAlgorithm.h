//
// Created by Polly Lilly on 21.11.17.
//

#pragma once

#include "utility.h"
#include <algorithm>

class GrahamScanAlgorithm {
public:
    static std::vector<Point> run(const std::vector<Point>& points, int subsetIdx, size_t);
};
