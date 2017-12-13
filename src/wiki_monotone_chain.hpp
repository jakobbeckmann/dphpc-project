/*
 * Monotone chain algorithm from the wiki implementation
 */


 // Implementation of Andrew's monotone chain 2D convex hull algorithm.
 // Asymptotic complexity: O(n log n).
 // Practical performance: 0.5-1.0 seconds for n=1000000 on a 1GHz machine.

#pragma once

#include <vector>
#include <algorithm>

#include "Point.h"
#include "utility.h"

class MonotoneChain {
public:
    static std::vector<Point> run(const std::vector<Point>& points, int parallel_idx, size_t parts);
};
