
#include <vector>
#include <omp.h>

#include "Jarvis_BinJarvis.hpp"
#include "JarvisAlgorithm.h"
#include "ChanAlgorithm.h"

std::vector<Point> Jarvis_BinJarvis::run(const std::vector<Point>& points, int parallel_idx, size_t parts) {
    std::vector<std::vector<Point> > hulls;
    hulls.resize(parts);

    omp_set_num_threads(parallel_idx);

    #pragma omp parallel for
    for (size_t i = 0; i < parts; ++i) {
        std::vector<Point> part = SplitVector(points, i, parts);
        hulls[i] = JarvisAlgorithm::run(part, 0/*unused*/, 0/*unused*/);
    }

    return ChanAlgorithm::mergeAllHulls(hulls);
}
