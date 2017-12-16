//
// Created by Polly Lilly on 23.11.17.
//

#include "JarvisAlgorithm.h"

std::vector<Point> JarvisAlgorithm::run(const std::vector<Point>& constPoints, int, size_t) {
    if (constPoints.size() <= 1) {
        return constPoints;
    }

    std::vector<Point> points = constPoints;
    size_t bottomLeftPointIdx = findLowestLeftmostPointIndex(points);

    Point curr_point;
    std::vector<Point> hull;
    size_t curr_idx = bottomLeftPointIdx, next_idx;

    do {
        curr_point = points[curr_idx];
        hull.push_back(curr_point);
        next_idx = (curr_idx + 1) % points.size();

        for (size_t idx = 0; idx < points.size(); idx++) {
            if (idx != curr_idx && idx != next_idx) {
                int linearity = getOrientation(points[curr_idx], points[idx], points[next_idx]);
                if (linearity == ANTICLOCKWISE || (linearity == COLLINEAR && getDistance(points[curr_idx], points[idx]) <
                                                                         getDistance(points[curr_idx], points[next_idx]))) {
                    next_idx = idx;

                }
            }
        }

        curr_idx = next_idx;

    } while (next_idx != bottomLeftPointIdx);

    return hull;
}
