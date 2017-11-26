/*
 * Quickhull class containing utility functions for algorithm
 */

#ifndef DPHPC_PROJECT_QUICKHULL_H
#define DPHPC_PROJECT_QUICKHULL_H

#include <vector>

#include "../../src/Point.h"

class Quickhull {
public:
    static void run(std::vector<Point>& input, std::vector<int>& hull_idxs);

private:
    /*
     * Gets leftmost / rightmost point in input.
     * Returns index of this point.
     */
    static int find_extreme(std::vector<Point>& input, bool left);

    /*
     * Subalgorithm to compute subhulls.
     */
    static void subquickhull(std::vector<Point>& input,
                             std::vector<int>& indeces,
                             std::vector<int>& output_idces,
                             const int first,
                             const int second);

    /*
     * Returns all points with a positive distance from the set and the point with
     * greatest distance to the line segment defined by first and second.
     */
    static std::tuple<std::vector<int>, int> get_dist_set(std::vector<Point>& input,
                                                          std::vector<int>& indeces,
                                                          const int first,
                                                          const int second);

    /*
     * Computes distance to point.
     * This uses a cross product to get relative distances. Hence the distances are not
     * real distances from the line segment but their relative sizes are accurate.
     *
     * result = 0: target is on line
     * result < 0: target is right of segment
     * result > 0: target is left of segment
     */
    static double compute_dist(std::vector<Point>& input,
                               const int target,
                               const int first,
                               const int second);
};


#endif // DPHPC_PROJECT_QUICKHULL_H
