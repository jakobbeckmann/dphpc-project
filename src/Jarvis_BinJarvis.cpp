
#include <vector>
#include <omp.h>

#include "Jarvis_BinJarvis.hpp"
#include "JarvisAlgorithm.h"
#include "ChanAlgorithm.h"

std::vector<Point> Jarvis_BinJarvis::run(const std::vector<Point>& points, int parallel_idx, size_t parts) {
    std::vector<std::vector<Point> > hulls;
    hulls.resize(parallel_idx);

    omp_set_num_threads(parallel_idx);

    #pragma omp parallel for
    for (size_t i = 0; i < parts; ++i) {
        std::vector<Point> part = SplitVector(points, i, parts);
        JarvisAlgorithm jarvis;
        hulls[i] = jarvis.run(part);
    }

    return ChanAlgorithm::mergeAllHulls(hulls);
}
