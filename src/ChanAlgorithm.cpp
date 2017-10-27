/**
chan2d.cpp
08-10-2017

@Description: 2D implementation of Chan's algorithm. Note that we do not consider a point to be part of the hull if it
is the middle point of 3 collinear points. Hence no three points in a hull (either global or local/Graham) will ever
be collinear.  The reason for this design choice is that collinear points only add computation for hull mergers.
They do output a different polygon but the shape of this output and the one with all collinear points is exactly
the same.
Note that this adds the restriction that we require the level of parallelism to be less than a third of the points.
Otherwise at least one subset of the points being analysed by Graham's algorithm will of size 2 and as we do not consider
collinear points to be part of hulls, Graham's scan returns an empty hull.
By extension we assume that the points have general position and hence no subset of points being analysed by Graham's
algorithm contains only collinear points.

@Author: Jakob Beckmann
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include "ChanAlgorithm.h"

/**
    Performs a Graham Scan on input vector of points.
    Returns a vector of points contained in the convex hull of the input.
    @param points: vector of points in the graham subset.
*/
vector<Point> ChanAlgorithm::grahamScan(vector<Point> &points, int subsetIdx) {
    FileWriter grahamWriter;
    grahamWriter.setGrahamSubsetIdx(subsetIdx);
    grahamWriter.setBaseName("graham_sub");
    grahamWriter.updateFileName();
    //grahamWriter.cleanExistingFiles();

    if (points.size() <= 1) {
        return points;
    }

    int bottomLeftPointIdx = findLowestLeftmostPointIndex(points);
    startingPoint = points[bottomLeftPointIdx];
    startingPoint.printPoint(),
    swapPoints(points, bottomLeftPointIdx, 0);

    sort( points.begin( ) + 1, points.end( ),
          [this]( Point p1, Point p2 )
          {
              return this->lowestAngleSort(p1, p2);
          });

    FileWriter::writePointsToFile(points, "all_sorted.dat");

    // Find the hull
    vector<Point> hull;
    vector<int> idxStack;

    hull.push_back(startingPoint);  // Adding first point to hull.
    idxStack.push_back(0);

    // Graham algorithm core
    for (int idx = 1; idx < points.size(); idx++) {
        //hull = checkHull(hull, points[idx], grahamWriter);

        Point base = points[idx];
        int last_idx = hull.size() - 1;

        while (hull.size() > 1 && getOrientation(hull[last_idx - 1], hull[last_idx], base) != ANTICLOCKWISE) {

            grahamWriter.writeGraham(hull[last_idx-1], hull[last_idx], base, 0, hull[last_idx], CLOCKWISE);


            hull.pop_back();
            idxStack.pop_back();

            last_idx = hull.size() - 1;

        }

        if (hull.empty() || hull[last_idx] != base) {
            grahamWriter.writeGraham(hull[last_idx-1], hull[last_idx], base, 1, base, ANTICLOCKWISE);
            hull.push_back(base);
            idxStack.push_back(idx);
        }
    }

    return hull;
}

/**
    Function used when sorting using qsort().
    Returns the point with lowest polar angle w.r.t startingPoint (which is being defined in grahamScan
    using the findLowestLeftMostPoint function).
    @param vpp1: pointer to first point
    @param vpp2: pointer to second point
*/
int ChanAlgorithm::lowestAngleSort(const Point& pp1, const Point& pp2) {
    //Point* pp1 = (Point*) vpp1;
    //Point* pp2 = (Point*) vpp2;

    int orient = getOrientation(startingPoint, pp1, pp2);
    if(orient == COLLINEAR) {
        return (getDistance(startingPoint, pp1) <= getDistance(startingPoint, pp2))? 1: 0;
    }
    return (orient == ANTICLOCKWISE)? 1: 0;
}


/**
    Returns the index of the point in the list that sits such that each other point
    in the list is on the left of the line from said point to the base point given in
    the arguments.
    @param points: vector of points forming a convex polygon.
    @param base: base point
*/
int findTangentIndex(const std::vector<Point>& points, Point base) {
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
            lower_bound = mid + 1;
        }
        lb_turn_after = getOrientation(base, points[lower_bound], points[(lower_bound + 1) % points.size()]);
    }
    return lower_bound;
}


/**
    Returns a pair representing the hull number and point index inside that hull. The represented
    point is the point with lowest y coordinate across all hulls.
    @param hulls: vector of hulls (vectors of points)
*/
std::pair<int, int> findLowestPoint(const std::vector<std::vector<Point> >& hulls) {
    int hull = 0, point = 0;
    double lowest_y = hulls[0][0].y;
    for (int hull_idx = 0; hull_idx < hulls.size(); hull_idx++) {
        for (int point_idx = 0; point_idx < hulls[hull_idx].size(); point_idx++) {
            if (hulls[hull_idx][point_idx].y < lowest_y) {
                hull = hull_idx;
                point = point_idx;
            }
        }
    }
    return make_pair(hull, point);
}


/**
    Returns the hull-point pair that will be used as the next point for the hull merge.
    @param hulls: vector of hulls (vectors of points)
    @param base_pair: base point (the last added point from the hull merge)
*/
std::pair<int, int> findNextMergePoint(const std::vector<std::vector<Point> >& hulls, std::pair<int, int> base_pair) {
    Point base = hulls[base_pair.first][base_pair.second];
    // Select next point on the same hull as the next point for the merge
    pair<int, int> result = make_pair(base_pair.first,
                                                (base_pair.second + 1) % hulls[base_pair.first].size());
    for (int hull_idx = 0; hull_idx < hulls.size(); hull_idx++) {
        if (hull_idx != base_pair.first) {
            int candidate_idx = findTangentIndex(hulls[hull_idx], base);
            Point previous = hulls[result.first][result.second];
            Point candidate = hulls[hull_idx][candidate_idx];
            int linearity = getOrientation(base, previous, candidate);
            if(linearity == CLOCKWISE || (linearity == COLLINEAR && getDistance(base, previous) <
                                                                            getDistance(base, candidate))) {
                result = make_pair(hull_idx, candidate_idx);
            }
        }
    }
    return result;
}


/**
    Returns the 2D convex hull of the points given in the argument. The parallelism index determines how many subsets of
    points should be analysed in parallel using Graham's scan.
    @param points: vector of points the be analysed
    @param parallel_idx: parallelism index determining the amount of parallel computation
*/
std::vector<Point> doChan(const std::vector<Point>& points, int parallel_idx) {
    std::vector<std::vector<Point> > hulls;
    /*
        TODO The following sode snipped will need to be parallelized using mpi in order to achieve real parallelism.
    */
    int currentSubsetIdx = 0;

    for (int idx = 0; idx < parallel_idx; idx++) {
        vector<Point> subset;
        for (int point_idx = idx; point_idx < points.size(); point_idx += parallel_idx) {
            subset.push_back(points[point_idx]);
        }
        hulls.push_back(grahamScan(subset, currentSubsetIdx));
        currentSubsetIdx++;
    }
    /*
        TODO END
    */
    pair<int, int> next_point = findLowestPoint(hulls);
    vector<Point> result;
    result.push_back(hulls[next_point.first][next_point.second]);
    do {
        next_point = findNextMergePoint(hulls, next_point);
        result.push_back(hulls[next_point.first][next_point.second]);
    } while (result[0] != result[result.size() - 1]);
    result.pop_back();
    return result;
}


