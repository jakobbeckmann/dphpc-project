#include "wiki_monotone_chain.hpp"

std::vector<Point> MonotoneChain::run(const std::vector<Point>& points_, int parallel_idx, size_t parts) {
    std::vector<Point> points = points_;
    int n = points.size(), k = 0;
	if (n == 1) return points;
	std::vector<Point> hull(2*n);

	// Sort points lexicographically
	std::sort(points.begin(), points.end());

	// Build lower hull
	for (int i = 0; i < n; ++i) {
		while (k >= 2 && cross(hull[k-2], hull[k-1], points[i]) <= 0) k--;
		hull[k++] = points[i];
	}

	// Build upper hull
	for (int i = n-2, t = k+1; i >= 0; i--) {
		while (k >= t && cross(hull[k-2], hull[k-1], points[i]) <= 0) k--;
		hull[k++] = points[i];
	}

	hull.resize(k-1);
	return hull;
}
