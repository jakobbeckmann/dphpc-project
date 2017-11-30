/**
ChanAlgorithm.cpp
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

@Authors: Jakob Beckmann, Matthäus Heer, Polena Lilyanova, Georg Kilzer
*/

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <omp.h>

#include "ChanAlgorithm.h"
#include "GrahamScanAlgorithm.h"

#ifndef WRITE_DEBUG
#define WRITE_DEBUG 0
#endif


/**
    Returns the index of the point in the list that sits such that each other point
    in the list is on the left of the line from said point to the base point given in
    the arguments.
    @param points: std::vector of points forming a convex polygon.
    @param base: base point
*/
int ChanAlgorithm::findTangentIndex(const std::vector<Point>& points, Point base) {
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
std::pair<int, int> ChanAlgorithm::findNextMergePoint(const std::vector<std::vector<Point>>& hulls, std::pair<int, int> base_pair) {
    Point base = hulls[base_pair.first][base_pair.second];

    // Select next point on the same hull as the next point for the merge
    std::pair<int, int> result = std::make_pair(base_pair.first, (base_pair.second + 1) % hulls[base_pair.first].size());

    for (int hull_idx = 0; hull_idx < hulls.size(); hull_idx++) {
        if (hull_idx != base_pair.first) {
            int candidate_idx = findTangentIndex(hulls[hull_idx], base);
            Point previous = hulls[result.first][result.second];
            Point candidate = hulls[hull_idx][candidate_idx];
            int linearity = getOrientation(base, previous, candidate);
            if(linearity == CLOCKWISE || (linearity == COLLINEAR && getDistance(base, previous) <
                                                                    getDistance(base, candidate))) {
                result = std::make_pair(hull_idx, candidate_idx);
            }
        }
    }

    return result;
}

std::vector<Point> ChanAlgorithm::mergeAllHulls(const std::vector<std::vector<Point>>& hulls)
{
    std::vector<Point> result;

    std::pair<int, int> next_point = findLowestPoint(hulls); // pair contains: subhullIdx and pointIdx on that hull
#if WRITE_DEBUG
    FileWriter mergeWriter = FileWriter("out_merge_hulls.dat");
    mergeWriter.writeMerge(hulls[next_point.first][next_point.second]);
#endif

    result.push_back(hulls[next_point.first][next_point.second]);  // lowest point of all sub hulls
    do {
        next_point = findNextMergePoint(hulls, next_point);
        result.push_back(hulls[next_point.first][next_point.second]);
#if WRITE_DEBUG
        mergeWriter.writeMerge(hulls[next_point.first][next_point.second]);
#endif
    } while (result[0] != result[result.size() - 1]);

    result.pop_back(); // due to previous double insertion of last point

    return result;
}

std::vector<Point> ChanAlgorithm::mergeTwoHulls(const std::vector<Point>& a, const std::vector<Point>& b)
{
	std::vector<Point> result;

	// TODO: Hack
	std::vector<std::vector<Point>> tmp = { a, b };

	return mergeAllHulls(tmp);
}


/**
    Returns the 2D convex hull of the points given in the argument. The parallelism index determines how many subsets of
    points should be analysed in parallel using Graham's scan.
    @param points: std::vector of points the be analysed
    @param parallel_idx: parallelism index determining the amount of parallel computation
*/
std::vector<Point> ChanAlgorithm::run(const std::vector<Point>& points, int parallel_idx, size_t parts) {

    std::vector<std::vector<Point> > hulls;
    hulls.resize(parts);

    omp_set_num_threads(parallel_idx);

    #pragma omp parallel for
    for (size_t i = 0; i < parts; ++i) {
        std::vector<Point> part = SplitVector(points, i, parts);
        hulls[i] = GrahamScanAlgorithm::run(part, i);
    }

    return mergeAllHulls(hulls);
}

/**
 * Splits the points into @p parts then computes convex hulls for each of the
 * sets in parallel (OpenMP) with Graham's scan.
 * Afterwards these individual hulls are merged as outlined below:
 * 0 1 2 3 4 5 6 7 8 index
 * A B C D E F G H I
 * AB  CD  EF  GH  I
 * ABCD    EFGH    I
 * ABCDEFGH        I
 * ABCDEFGHI
 *
 * Where the merges from one line to the next are performed in parallel with
 * OpenMP again.
*/
std::vector<Point> ChanAlgorithm2Merge::run(const std::vector<Point>& points, int parallel_idx, size_t parts)
{
	std::vector<std::vector<Point> > hulls;
	hulls.resize(parts);

	omp_set_num_threads(parallel_idx);

	#pragma omp parallel for
	for (size_t i = 0; i < parts; ++i) {
		std::vector<Point> part = SplitVector(points, i, parts);
		hulls[i] = GrahamScanAlgorithm::run(part, i);
	}

	// step_size = 2, 4, 8, ...
	for (size_t i = hulls.size(), step_size = 2; step_size/2 < i; step_size <<= 1)
	{
		// The loop operates on j and j+step_size/2, but iterates over j+step_size/2
		// this is needed to fit openmp loop restrictions.
		#pragma omp parallel for
		for (size_t j_h = step_size/2; j_h < i; j_h += step_size)
		{
			size_t j = j_h-step_size/2; // 0, step_size, 2*step_size, 3*step_size, ...
			hulls[j] = mergeTwoHulls(hulls[j], hulls[j_h]);
		}
	}

	return hulls[0];
}
