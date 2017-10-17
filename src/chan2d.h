
#ifndef DPHPC_PROJECT_CHAN2D_H
#define DPHPC_PROJECT_CHAN2D_H

std::vector<Point> hull_check(std::vector<Point>& points, Point base);
std::vector<Point> graham_scan(std::vector<Point>& points);
int tangent_idx(std::vector<Point> points, Point base);
std::pair<int, int> lowest_point(std::vector<std::vector<Point> > hulls);
std::pair<int, int> next_merge_point(std::vector<std::vector<Point> > hulls, std::pair<int, int> base_pair);
std::vector<Point> chan(std::vector<Point> points, int parallel_idx);

#endif //DPHPC_PROJECT_CHAN2D_H
