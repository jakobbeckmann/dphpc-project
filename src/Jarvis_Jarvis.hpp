/*
 * Jarvis - jarvis class containing utility functions for algorithm
 */

#pragma once

#include <vector>
#include <omp.h>

#include "Point.h"
#include "JarvisAlgorithm.h"

class Jarvis_Jarvis {
public:
    static std::vector<Point> run(const std::vector<Point>& points, int parallel_idx, size_t parts);
};
