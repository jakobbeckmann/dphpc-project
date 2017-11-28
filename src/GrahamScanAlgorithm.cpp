//
// Created by Polly Lilly on 21.11.17.
//

#include "GrahamScanAlgorithm.h"

/**
    Retruns list of points belonging to a convex hull after running Graham Scan algorithm.
 */
std::vector<Point> GrahamScanAlgorithm::run(const std::vector<Point>& constPoints) {
    if (constPoints.size() <= 1) {
        return constPoints;
    }

    std::vector<Point> points = constPoints;

    int bottomLeftPointIdx = findLowestLeftmostPointIndex(points);
    std::swap(points[bottomLeftPointIdx], points[0]);
    const Point& startingPoint = points[0];

    // Returns the point with lowest polar angle w.r.t startingPoint
    std::stable_sort(points.begin()+1, points.end(), [&startingPoint](const Point& p1, const Point& p2) {
        int orient = getOrientation(startingPoint, p1, p2);
        if(orient == COLLINEAR) {
            return getDistance(startingPoint, p1) <= getDistance(startingPoint, p2);
        }
        return orient != CLOCKWISE;
    });

    // Find the hull
    std::vector<Point> hull;
    std::vector<int> idxStack;

    hull.push_back(startingPoint);  // Adding first point to hull.
    idxStack.push_back(0);

    // Graham algorithm core
    for (int idx = 1; idx < points.size(); idx++) {

        Point base = points[idx];
        int last_idx = hull.size() - 1;

        while (hull.size() > 1 && getOrientation(hull[last_idx - 1], hull[last_idx], base) != ANTICLOCKWISE) {

            hull.pop_back();
            idxStack.pop_back();

            last_idx = hull.size() - 1;

        }

        if (hull.empty() || hull[last_idx] != base) {
            hull.push_back(base);
            idxStack.push_back(idx);
        }
    }

    return hull;
}
