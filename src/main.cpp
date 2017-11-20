/**
    Main function for the chan 2d algorithm implementation.
 */

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>


#include "Point.h"
#include "PointsGenerator/ShapeGenerator.h"
#include "utility.h"
#include "ChanAlgorithm.h"

int main(int argc, char const *argv[]) {

    //-------------------------------------------------------------------------------
    // CLEAR OUTPUT FOLDER



    // USER INPUT SECTION

    int RANGE = 10;
    size_t POINT_COUNT = 0;
    size_t PARALLELISM_IDX = 0;

    if (argc == 3) {
        std::stringstream sstream(argv[1]);
		sstream >> PARALLELISM_IDX;
    } else {
        std::cout << "Enter number of subsets for Graham Scans... " << std::endl;
        std::cin >> PARALLELISM_IDX;
    }
    if(PARALLELISM_IDX < 1) return -1;
    FileWriter::writeNumberToFile(PARALLELISM_IDX, "../Output/out_n_graham_subs.dat", true);

    if (argc == 3) {
        std::stringstream sstream(argv[2]);
        sstream >> POINT_COUNT;
    } else {
        std::cout << "Enter number of points..." << std::endl;
        std::cin >> POINT_COUNT;
    }
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
    std::vector<Point> points = randomShapeGenerator(POINT_COUNT, RANGE);
    std::vector<Point> result = chan.run(points, PARALLELISM_IDX);

    for (std::vector<Point>::iterator it = result.begin(); it != result.end(); ++it) {
        it->print();
    }

    //-------------------------------------------------------------------------------
    // OUTPUT SECTION

    std::cout << "\n\n=========Result=========" << std::endl;
    for(Point point: result) {
        std::cout << point << " ";
    }
    std::cout << std::endl;

    FileWriter::writePointsToFile(result, "../Output/out_hull_points.dat", true);

    return 0;
}
