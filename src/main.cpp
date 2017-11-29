/**
 *
 * Main function to call different algorithms.
 *
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include <sstream>

#include "timer.h"
#include "Point.h"
#include "ChanAlgorithm.h"
#include "GrahamScanAlgorithm.h"
#include "JarvisAlgorithm.h"
#include "Quickhull.hpp"
#include "Jarvis_Jarvis.hpp"
#include "Jarvis_BinJarvis.hpp"
#include "Jarvis_Graham.hpp"
#include "Jarvis_Quickhull.hpp"
#include "Graham_Jarvis.hpp"
#include "Graham_Graham.hpp"
#include "Graham_Quickhull.hpp"


int main(int argc, char const *argv[]) {
    #ifdef WRITE_DEBUG
        std::cout << "Running in DEBUG mode - writing ALL output files." << std::endl;
    #endif

    if (argc < 5) {
        std::cout << "Usage: " << " n_cores input_file algorithm iter_idx\n";
        std::cout << "  algorithm := chan_normal | chan_merge_var | graham | jarvis \n";

        return -1;
    }

    Timer timer;
    Timer file_write_timer;
    Timer file_read_timer;
    Timer total_timer;

    total_timer.start();

    int n_cores             = atoi(argv[1]);
    auto numberOfCores      = (size_t)n_cores;
    std::string inputFile   = argv[2];
    std::string algorithm   = argv[3];
    int iterIdx             = atoi(argv[4]);

    file_read_timer.start();
    std::vector<Point> points = readPointsFromFile(inputFile);
    file_read_timer.stop();

    std::vector<Point> result;

    if (algorithm == "chan_normal") {
        timer.start();
        result = ChanAlgorithm::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else if (algorithm == "chan_merge_var") {
        timer.start();
        result = ChanAlgorithm2Merge::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else  if (algorithm == "graham") {
        timer.start();
        result = GrahamScanAlgorithm::run(points, 0);
        timer.stop();

    } else  if (algorithm == "jarvis") {
        timer.start();
        result = JarvisAlgorithm::run(points);
        timer.stop();

    } else  if (algorithm == "jarvis_jarvis") {
        timer.start();
        result = Jarvis_Jarvis::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else  if (algorithm == "jarvis_binjarvis") {
        timer.start();
        result = Jarvis_BinJarvis::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else  if (algorithm == "jarvis_graham") {
        timer.start();
        result = Jarvis_Graham::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else  if (algorithm == "jarvis_quickhull") {
        timer.start();
        result = Jarvis_Quickhull::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else  if (algorithm == "graham_jarvis") {
        timer.start();
        result = Graham_Jarvis::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else  if (algorithm == "graham_graham") {
        timer.start();
        result = Graham_Graham::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else  if (algorithm == "graham_quickhull") {
        timer.start();
        result = Graham_Quickhull::run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else if (algorithm == "quickhull") {
        std::vector<int> result_idxs;
        timer.start();
        Quickhull::run(points, result_idxs);
        for(int idx: result_idxs) {
            result.push_back(points[idx]);
        }
        timer.stop();

    } else {
        std::cout << "No such algorithm!";
        std::exit(EXIT_FAILURE);
    }

    std::cout << "\n\n=========Result=========" << std::endl;
    std::cout << "Algorithm:     " << algorithm << std::endl;
    std::cout << "Input size:    " << points.size() << std::endl;
    std::cout << "N hull points: " << result.size() << std::endl;
    std::cout << "Iteration:     " << iterIdx << std::endl;
    std::cout << "Time used:     " << timer.get_timing() << std::endl;

    file_write_timer.start();
    std::stringstream fileName;
    fileName << "hull_points_" << iterIdx << ".dat";
    FileWriter::writePointsToFile(result, fileName.str(), true);

    timer.write_to_file(iterIdx);
    file_write_timer.stop();
    std::cout << "Write time:    " << file_write_timer.get_timing() << std::endl;
    std::cout << "Read time:     " << file_read_timer.get_timing() << std::endl;

    total_timer.stop();
    std::cout << "Total time:    " << total_timer.get_timing() << "\n" << std::endl;

    return 0;
}
