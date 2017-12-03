/*
 * Quickhull class containing utility functions for algorithm
 */

#pragma once

#include <vector>

#include "Point.h"

class Quickhull {
public:
    static void run(const std::vector<Point>& input, std::vector<int>& hull_idxs);
    static std::vector<Point> runAndReturnPoints(const std::vector<Point>& points, int parallel_idx, size_t parts);
private:
    /*
     * Gets leftmost / rightmost point in input.
     * Returns index of this point.
     */
    static int find_extreme(const std::vector<Point>& input, bool left);

    /*
     * Subalgorithm to compute subhulls.
     */
    static void subquickhull(const std::vector<Point>& input,
                             std::vector<int>& indeces,
                             std::vector<int>& output_idces,
                             int first,
                             int second);

    /*
     * Returns all points with a positive distance from the set and the point with
     * greatest distance to the line segment defined by first and second.
     */
    static std::tuple<std::vector<int>, int> get_dist_set(const std::vector<Point>& input,
                                                          std::vector<int>& indeces,
                                                          int first,
                                                          int second);

    /*
     * Computes distance to point.
     * This uses a cross product to get relative distances. Hence the distances are not
     * real distances from the line segment but their relative sizes are accurate.
     *
     * result = 0: target is on line
     * result < 0: target is right of segment
     * result > 0: target is left of segment
     */
    static double compute_dist(const std::vector<Point>& input,
                               int target,
                               int first,
                               int second);
};
