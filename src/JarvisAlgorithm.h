//
// Created by Polly Lilly on 23.11.17.
//

#ifndef DPHPC_PROJECT_JARVISALGORITHM_H
#define DPHPC_PROJECT_JARVISALGORITHM_H

#include "utility.h"
#include <algorithm>

class JarvisAlgorithm {
private:
    int findTangentIndex(const std::vector<Point>& points, Point base);
    int findNextPoint(const std::vector<Point>& points, Point base, int base_idx);
public:
    std::vector<Point> run(const std::vector<Point>& points, size_t parallel_idx);
};


#endif //DPHPC_PROJECT_JARVISALGORITHM_H
