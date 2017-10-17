/**
    Main function for the chan 2d algorithm implementation.
 */

#include <iostream>
#include <vector>

#include "Point.h"
#include "chan2d.h"

int main(int argc, char const *argv[]) {
    int POINT_COUNT = 0, PARALLELISM_IDX = 1;
    double x = 0, y = 0;

    std::cout << "Please enter the parallelism index:" << std::endl;
    std::cin >> PARALLELISM_IDX;
    if(PARALLELISM_IDX < 1) return -1;

    std::cout << "Please enter the total number of points:" << std::endl;
    std::cin >> POINT_COUNT;
    if(POINT_COUNT < PARALLELISM_IDX * 3) {
        std::cout << "Please need more points or less parallelism." << std::endl;
        return -1;
    }

    //TODO: Random point generator
    std::vector<Point> points;
    for(int idx = 0; idx < POINT_COUNT; idx++) {
        std::cout << "Coordinates of point " << idx + 1 << ": ";
        std::cin >> x >> y;
        points.push_back(Point(x, y));
    }

    // std::vector<Point> result = graham_scan(points);
    std::vector<Point> result = chan(points, PARALLELISM_IDX);
    std::cout << "=========Result=========" << std::endl;
    for(Point point: result) {
        std::cout << point << " ";
    }
    std::cout << std::endl;
    return 0;
}
