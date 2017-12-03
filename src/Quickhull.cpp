/*
 * Implementation of the quickhull algorithm.
 */

#include <tuple>
#include "Quickhull.hpp"


void Quickhull::run(const std::vector<Point>& input, std::vector<int>& hull_idxs) {
    int point_min_x = find_extreme(input, true);
    int point_max_x = find_extreme(input, false);

    std::vector<int> indeces;
    for(int idx = 0; idx < input.size(); idx++) {
        indeces.push_back(idx);
    }

    hull_idxs.push_back(point_min_x);
    hull_idxs.push_back(point_max_x);

    subquickhull(input,
                 indeces,
                 hull_idxs,
                 point_min_x,
                 point_max_x);
    subquickhull(input,
                 indeces,
                 hull_idxs,
                 point_max_x,
                 point_min_x);
}

std::vector<Point> Quickhull::runAndReturnPoints(const std::vector<Point>& points, int parallel_idx, size_t parts) {
    std::vector<int> result;

    Quickhull::run(points, result);

    std::vector<Point> result_points;
    for(int idx: result) {
        result_points.push_back(points[idx]);
    }

    return result_points;
}

int Quickhull::find_extreme(const std::vector<Point>& input, bool left) {
    int pivot_idx = 0;
    for(int idx = 0; idx < input.size(); idx++) {
        if((left && input[idx].x < input[pivot_idx].x) ||
                (!left && input[idx].x > input[pivot_idx].x)) {
            pivot_idx = idx;
        }
    }
    return pivot_idx;
}

void Quickhull::subquickhull(const std::vector<Point>& input,
                             std::vector<int>& indeces,
                             std::vector<int>& output_idxs,
                             const int first,
                             const int second) {

    std::tuple<std::vector<int>, int> results = get_dist_set(input,
                                                             indeces,
                                                             first,
                                                             second);

    if(std::get<1>(results) != -1) {
        output_idxs.push_back(std::get<1>(results));
        subquickhull(input,
                     std::get<0>(results),
                     output_idxs,
                     first,
                     std::get<1>(results));
        subquickhull(input,
                     std::get<0>(results),
                     output_idxs,
                     std::get<1>(results),
                     second);
    }
}

double Quickhull::compute_dist(const std::vector<Point>& input,
                    const int target,
                    const int first,
                    const int second) {
    // Uses form of cross product
    double segment_x = input[second].x - input[first].x;
    double segment_y = input[second].y - input[first].y;

    double vec_target_x = input[target].x - input[first].x;
    double vec_target_y = input[target].y - input[first].y;

    return vec_target_y * segment_x - vec_target_x * segment_y;
}

std::tuple<std::vector<int>, int> Quickhull::get_dist_set(const std::vector<Point>& input,
                                                          std::vector<int>& indeces,
                                                          const int first,
                                                          const int second) {
    std::vector<int> result;

    int idx_max_distance = -1;
    double max_distance = 0;

    for(int idx: indeces) {
        double temp_distance = compute_dist(input, idx, first, second);

        if(temp_distance > 0) {
            result.push_back(idx);
            if(temp_distance > max_distance) {
                idx_max_distance = idx;
                max_distance = temp_distance;
            }
        }
    }

    return std::make_tuple(result, idx_max_distance);
}
