/*
 * Quickhull - graham class containing utility functions for algorithm
 */

#pragma once

#include <vector>
#include <omp.h>

#include "Point.h"
#include "GrahamScanAlgorithm.h"
#include "Quickhull.hpp"

class Quickhull_Graham {
public:
    static std::vector<Point> run(const std::vector<Point>& points, int parallel_idx, size_t parts);
};
