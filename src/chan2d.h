
#ifndef DPHPC_PROJECT_CHAN2D_H
#define DPHPC_PROJECT_CHAN2D_H

std::vector<Point> checkHull(std::vector<Point> &points, Point base);
std::vector<Point> doGrahamScan(std::vector<Point> &points);
int findTangentIndex(const std::vector<Point>& points, Point base);
std::pair<int, int> findLowestPoint(const std::vector<std::vector<Point> >& hulls);
std::pair<int, int> findNextMergePoint(const std::vector<std::vector<Point> >& hulls, std::pair<int, int> base_pair);
std::vector<Point> doChan(const std::vector<Point>& points, int parallel_idx);

#endif //DPHPC_PROJECT_CHAN2D_H
