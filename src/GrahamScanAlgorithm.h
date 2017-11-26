//
// Created by Polly Lilly on 21.11.17.
//

#ifndef DPHPC_PROJECT_GRAHAMSCANALGORITHM_H
#define DPHPC_PROJECT_GRAHAMSCANALGORITHM_H

#include "utility.h"
#include <algorithm>

class GrahamScanAlgorithm {
public:
    std::vector<Point> run(const std::vector<Point>& points, size_t parallel_idx);
};


#endif //DPHPC_PROJECT_GRAHAMSCANALGORITHM_H
