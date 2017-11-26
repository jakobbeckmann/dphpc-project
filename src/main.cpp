/**
    Main function for the chan 2d algorithm implementation.
 */

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <omp.h>

#include "ChanAlgorithm.h"
#include "timer.hpp"


int main(int argc, char const *argv[]) {

    std::cout << "[Information]: Max n threads possible - " << omp_get_max_threads() << std::endl;

    //-------------------------------------------------------------------------------
    // USER INPUT SECTION
    std::string inputFile = "../Input/bird_points.dat";
    std::vector<int> nCores = {1, 2, 3, 4, 5, 6, 7, 8};


    //-------------------------------------------------------------------------------
    // ALGORITHM SECTION
    std::vector<Point> points = readPointsFromFile(inputFile);
    size_t PARALLELISM_IDX = nCores.size();
    ChanAlgorithm chan;
    timer t;
    t.clean_timing_file();

    std::vector<std::vector<Point>> allResults;

    for (int threadCount: nCores)
    {
        omp_set_dynamic(0);                 // Explicitly disable dynamic teams
        omp_set_num_threads(threadCount);

        t.start();
        std::vector<Point> result = chan.run(points, PARALLELISM_IDX);
        t.stop();
        allResults.emplace_back(result);
        std::cout << "Chan algorithm took:" << t.get_timing() << " seconds." << std::endl;
        t.write_to_file(threadCount);

    }

    //-------------------------------------------------------------------------------
    // OUTPUT SECTION

    for (std::vector<Point> result: allResults)
    {
        std::cout << "\n\n=========Result=========" << std::endl;
        std::cout << " " << result.size() << " hull points:" << std::endl;
        for(Point point: result) {
            std::cout << point << " ";
        }
        std::cout << std::endl;
        FileWriter::writePointsToFile(result, "../Output/out_hull_points.dat", true);
    }


    return 0;
}
