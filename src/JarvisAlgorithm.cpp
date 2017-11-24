//
// Created by Polly Lilly on 23.11.17.
//

#include "JarvisAlgorithm.h"

/**
    Returns the index of the point in the list that sits such that each other point
    in the list is on the left of the line from said point to the base point given in
    the arguments.
    @param points: std::vector of points forming a convex polygon.
    @param base: base point
*/
int JarvisAlgorithm::findTangentIndex(const std::vector<Point>& points, Point base) {
    int lower_bound = 0;
    int upper_bound = points.size();

    // Find getOrientation of turns for points right before and after the lower bound.
    int lb_turn_before = getOrientation(base, points[0], points[points.size() - 1]);
    int lb_turn_after = getOrientation(base, points[0], points[1]);

    // Check if first point is the point lying to the right of all other points.
    if (lb_turn_before != CLOCKWISE && lb_turn_after == ANTICLOCKWISE) {
        return 0;
    }

    // First point is not the right most point.
    while (lower_bound < upper_bound) {
        // Find index of point in between the two bounds.
        int mid = (upper_bound + lower_bound) / 2;

        // Compute its turns.
        int mid_turn_before = getOrientation(base, points[mid], points[(mid - 1) % points.size()]);
        int mid_turn_after = getOrientation(base, points[mid], points[(mid + 1) % points.size()]);

        // Check in which direction the next cut should be based on the position relative to
        // the lower_bound point.
        int cut_direction = getOrientation(base, points[lower_bound], points[mid]);

        if (mid_turn_before != CLOCKWISE && mid_turn_after == ANTICLOCKWISE) {
            // All points lie to the right of 'mid'
            return mid;
        } else if ((cut_direction != CLOCKWISE && lb_turn_after != ANTICLOCKWISE) ||
                   (cut_direction == CLOCKWISE && mid_turn_before == CLOCKWISE)) {
            // The leftmost point lies to the right of the cut (line between 'lower_bound' and 'mid')
            upper_bound = mid;
        } else {
            // The leftmost point lies to the left of the cut.
            lower_bound = mid;
        }

        lb_turn_after = getOrientation(base, points[lower_bound], points[(lower_bound + 1) % points.size()]);
    }

    return lower_bound;
}

/**
    Returns the hull-point std::pair that will be used as the next point for the hull merge.
    @param hulls: std::vector of hulls (vectors of points)
    @param base_pair: base point (the last added point from the hull merge)
*/
int JarvisAlgorithm::findNextPoint(const std::vector<Point>& points, Point base, int base_idx) {
    int result = (base_idx + 1)%points.size();

    for (int idx = 0; idx < points.size(); idx++) {
        if (idx != base_idx) {
            int candidate_idx = findTangentIndex(points, base);
            Point previous = points[result];
            Point candidate = points[candidate_idx];
            int linearity = getOrientation(base, previous, candidate);
            if(linearity == CLOCKWISE || (linearity == COLLINEAR && getDistance(base, previous) <
                                                                    getDistance(base, candidate))) {
                result = candidate_idx;
            }
        }
    }

    return result;
}

/**
        Return list of points belonging to a convex hull after running Jarvis algorithm.
 */
std::vector<Point> JarvisAlgorithm::run(const std::vector<Point>& constPoints, size_t parallel_idx) {
    if (constPoints.size() <= 1) {
        return constPoints;
    }

    std::vector<Point> points = constPoints;
    int bottomLeftPointIdx = findLowestLeftmostPointIndex(points);

    Point next_point;
    std::vector<Point> hull;
    int next_idx = bottomLeftPointIdx;

    do {
        next_point = points[next_idx];
        hull.push_back(next_point);
        next_idx = findNextPoint(points, next_point, next_idx);
    } while (hull[bottomLeftPointIdx] != hull[hull.size() - 1]);

    hull.pop_back(); // due to previous double insertion of last point
    return hull;
}

