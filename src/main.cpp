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

    //-------------------------------------------------------------------------------
    // CREATE OUTPUT FOLDER

    // TODO: Platform specific, therefore use boost library, include into project

    //-------------------------------------------------------------------------------
    // USER INPUT SECTION

    int POINT_COUNT = 0,  MIN = -10, MAX = 10;
    size_t PARALLELISM_IDX = 0;
    std::cout << "Enter number of subsets for Graham Scans..." << std::endl;
    std::cin >> PARALLELISM_IDX;
    if(PARALLELISM_IDX < 1) return -1;

    std::cout << "Enter number of points..." << std::endl;
    std::cin >> POINT_COUNT;
    if(POINT_COUNT < PARALLELISM_IDX * 3) {
        std::cout << "Please need more points or less parallelism." << std::endl;
        return -1;
    }

    /*
    std::cout << "Enter MIN value for x/y coordinates..." << std::endl;
    std::cin >> MIN;
    std::cout << "Enter MAX value for x/y coordinates..." << std::endl;
    std::cin >> MAX;
    */

    //-------------------------------------------------------------------------------
    // ALGORITHM SECTION

    ChanAlgorithm chan;
    std::vector<Point> points = createPoints(POINT_COUNT, MIN, MAX);
    std::vector<Point> result = chan.run(points, PARALLELISM_IDX);

    //-------------------------------------------------------------------------------
    // OUTPUT SECTION

    std::cout << "\n\n=========Result=========" << std::endl;
    for(Point point: result) {
        std::cout << point << " ";
    }
    std::cout << std::endl;

    FileWriter::writePointsToFile(result, "hull_points.dat", true);

    return 0;
}
