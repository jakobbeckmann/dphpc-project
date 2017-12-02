
#include <vector>
#include <omp.h>

#include "Graham_Graham.hpp"

std::vector<Point> Graham_Graham::run(const std::vector<Point>& points, int parallel_idx, size_t parts) {
    std::vector<std::vector<Point> > hulls;
    hulls.resize(parts);

    omp_set_num_threads(parallel_idx);

    #pragma omp parallel for
    for (size_t i = 0; i < parts; ++i) {
        std::vector<Point> part = SplitVector(points, i, parts);
        hulls[i] = GrahamScanAlgorithm::run(part, i, 0/*unused*/);
    }

    return GrahamScanAlgorithm::run(MergeVectors(std::move(hulls)), 0/*unused*/, 0/*unused*/);
}
