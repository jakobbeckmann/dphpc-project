
#include <vector>
#include <omp.h>

#include "Quickhull_BinJarvis.hpp"

std::vector<Point> Quickhull_BinJarvis::run(const std::vector<Point>& points, int parallel_idx, size_t parts) {
    std::vector<std::vector<Point> > hulls;
    hulls.resize(parts);

    omp_set_num_threads(parallel_idx);

    #pragma omp parallel for
    for (size_t i = 0; i < parts; ++i) {
        std::vector<int> result_idxs;
        std::vector<Point> part = SplitVector(points, i, parts);
        Quickhull::run(part, result_idxs);
        for(int idx: result_idxs) {
            hulls[i].push_back(part[idx]);
        }

        // Order the points for binary jarvis
        int bottomLeftPointIdx = findLowestLeftmostPointIndex(hulls[i]);
        std::swap(hulls[i][bottomLeftPointIdx], hulls[i][0]);
        const Point& startingPoint = hulls[i][0];

        // Returns the point with lowest polar angle w.r.t startingPoint
        std::stable_sort(hulls[i].begin()+1, hulls[i].end(), [&startingPoint](const Point& p1, const Point& p2) {
            int orient = getOrientation(startingPoint, p1, p2);
            if(orient == COLLINEAR) {
                return getDistance(startingPoint, p1) <= getDistance(startingPoint, p2);
            }
            return orient != CLOCKWISE;
        });
    }

    return ChanAlgorithm::mergeAllHulls(hulls);
}
