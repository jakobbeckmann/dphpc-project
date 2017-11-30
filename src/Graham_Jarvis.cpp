
#include <vector>
#include <omp.h>

#include "Graham_Jarvis.hpp"

std::vector<Point> Graham_Jarvis::run(const std::vector<Point>& points, int parallel_idx, size_t parts) {
    std::vector<std::vector<Point> > hulls;
    hulls.resize(parts);

    omp_set_num_threads(parallel_idx);

    #pragma omp parallel for
    for (size_t i = 0; i < parts; ++i) {
        std::vector<Point> part = SplitVector(points, i, parts);
        hulls[i] = GrahamScanAlgorithm::run(part, i);
    }

    std::vector<Point> hull_points;
    for(std::vector<Point> hull: hulls) {
        for(Point point: hull) {
            hull_points.push_back(point);
        }
    }

    return JarvisAlgorithm::run(hull_points);
}
