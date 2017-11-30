
#include <vector>
#include <omp.h>

#include "Jarvis_Jarvis.hpp"
#include "JarvisAlgorithm.h"

std::vector<Point> Jarvis_Jarvis::run(const std::vector<Point>& points, int parallel_idx, size_t parts) {
    std::vector<std::vector<Point> > hulls;
    hulls.resize(parts);

    omp_set_num_threads(parallel_idx);

    #pragma omp parallel for
    for (size_t i = 0; i < parts; ++i) {
        std::vector<Point> part = SplitVector(points, i, parts);
        hulls[i] = JarvisAlgorithm::run(part, 0/*unused*/, 0/*unused*/);
    }

    std::vector<Point> hull_points;
    for(std::vector<Point> hull: hulls) {
        for(Point point: hull) {
            hull_points.push_back(point);
        }
    }

    return JarvisAlgorithm::run(hull_points, 0/*unused*/, 0/*unused*/);
}
