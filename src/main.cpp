/**
    Main function for the chan 2d algorithm implementation.
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <random>

#include "Point.h"
#include "utility.h"
#include "ChanAlgorithm.h"



int main(int argc, char const *argv[]) {
    int POINT_COUNT = 0, PARALLELISM_IDX = 1, MIN = -10, MAX = 10;


    ChanAlgorithm chanAlgo;


    //TODO: Parallel implementation
    std::cout << "Please enter the parallelism index:" << std::endl;
    std::cin >> PARALLELISM_IDX;
    if(PARALLELISM_IDX < 1) return -1;

    std::cout << "Please enter the total number of points:" << std::endl;
    std::cin >> POINT_COUNT;
    if(POINT_COUNT < PARALLELISM_IDX * 3) {
        std::cout << "Please need more points or less parallelism." << std::endl;
        return -1;
    }

    std::cout << "Please enter the possible min and max value for x/y coordinates." << std::endl;
    std::cin >> MIN;
    std::cin >> MAX;

    std::vector<Point> points = createPoints(POINT_COUNT, MIN, MAX);

    // std::vector<Point> result = doGrahamScan(points);
    std::vector<Point> result = chanAlgo.run(points, PARALLELISM_IDX);
    std::cout << "=========Result=========" << std::endl;
    for(Point point: result) {
        std::cout << point << " ";
    }
    std::cout << std::endl;

    FileWriter::writePointsToFile(result, "hull_points.dat");

    return 0;
}
