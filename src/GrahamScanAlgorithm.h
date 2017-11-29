//
// Created by Polly Lilly on 21.11.17.
//

#pragma once

#include "utility.h"
#include <algorithm>

class GrahamScanAlgorithm {
public:
    static std::vector<Point> run(std::vector<Point>& points, int subsetIdx);
};
