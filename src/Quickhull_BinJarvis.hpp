/*
 * Quickhull - binary jarvis class containing utility functions for algorithm
 */

#pragma once

#include <vector>
#include <algorithm>
#include <omp.h>

#include "utility.h"
#include "Point.h"
#include "ChanAlgorithm.h"
#include "Quickhull.hpp"

class Quickhull_BinJarvis {
public:
    static std::vector<Point> run(const std::vector<Point>& points, int parallel_idx, size_t parts);
};
