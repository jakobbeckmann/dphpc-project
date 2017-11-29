/*
 * Jarvis - binray jarvis class containing utility functions for algorithm
 */

#pragma once

#include <vector>
#include <omp.h>

#include "Point.h"
#include "JarvisAlgorithm.h"
#include "ChanAlgorithm.h"

class Jarvis_BinJarvis {
public:
    static std::vector<Point> run(const std::vector<Point>& points, int parallel_idx, size_t parts);
};
