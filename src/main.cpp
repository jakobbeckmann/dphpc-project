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
#include "Quickhull_BinJarvis.hpp"
#include "Quickhull_Jarvis.hpp"
#include "Quickhull_Graham.hpp"
#include "Quickhull_Quickhull.hpp"


int main(int argc, char const *argv[]) {
    #ifdef WRITE_DEBUG
        std::cout << "Running in DEBUG mode - writing ALL output files.\n";
    #endif

    if (argc < 6) {
        std::cout << "Usage: " << " n_cores part_size input_file algorithm iter_idx\n"
                     "  algorithm := "
                                    "chan_normal"
                                 " | chan_merge_var"
                                 " | graham"
                                 " | jarvis"
                                 " | jarvis_jarvis"
                                 " | jarvis_binjarvis"
                                 " | jarvis_graham"
                                 " | jarvis_quickhull"
                                 " | graham_jarvis"
                                 " | graham_graham"
                                 " | graham_quickhull"
                                 " | quickhull_binjarvis"
                                 " | quickhull_jarvis"
                                 " | quickhull_graham"
                                 " | quickhull_quickhull"
                                 " | quickhull"
                                 "\n";

        return -1;
    }

    Timer timer;
    Timer file_write_timer;
    Timer file_read_timer;
    Timer total_timer;

    total_timer.start();

    int n_cores             = atoi(argv[1]);
    auto part_size        = size_t(atoi(argv[2]));
    auto numberOfCores      = (size_t)n_cores;
    std::string inputFile   = argv[3];
    std::string algorithm   = argv[4];
    int iterIdx             = atoi(argv[5]);


    file_read_timer.start();
    std::vector<Point> points = readPointsFromFile(inputFile);
    file_read_timer.stop();

    std::vector<Point> result;

    if (algorithm == "chan_normal") {
        timer.start();
        result = ChanAlgorithm::run(points, numberOfCores, part_size);
        timer.stop();

    } else if (algorithm == "chan_merge_var") {
        timer.start();
        result = ChanAlgorithm2Merge::run(points, numberOfCores, part_size);
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
        result = Jarvis_Jarvis::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "jarvis_binjarvis") {
        timer.start();
        result = Jarvis_BinJarvis::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "jarvis_graham") {
        timer.start();
        result = Jarvis_Graham::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "jarvis_quickhull") {
        timer.start();
        result = Jarvis_Quickhull::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "graham_jarvis") {
        timer.start();
        result = Graham_Jarvis::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "graham_graham") {
        timer.start();
        result = Graham_Graham::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "graham_quickhull") {
        timer.start();
        result = Graham_Quickhull::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "quickhull_binjarvis") {
        timer.start();
        result = Quickhull_BinJarvis::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "quickhull_jarvis") {
        timer.start();
        result = Quickhull_Jarvis::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "quickhull_graham") {
        timer.start();
        result = Quickhull_Graham::run(points, numberOfCores, part_size);
        timer.stop();

    } else  if (algorithm == "quickhull_quickhull") {
        timer.start();
        result = Quickhull_Quickhull::run(points, numberOfCores, part_size);
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
        std::cout << std::string("No such algorithm! Given ") + algorithm;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "\n\n=========Result=========\n"
              << "Algorithm:     " << algorithm << "\n"
              << "Input size:    " << points.size() << "\n"
              << "N hull points: " << result.size() << "\n"
              << "Iteration:     " << iterIdx << "\n"
              << "Time used:     " << timer.get_timing() << "\n";

    file_write_timer.start();
    std::stringstream fileName;
    fileName << "hull_points_" << iterIdx << ".dat";
    FileWriter::writePointsToFile(result, fileName.str(), true);

    timer.write_to_file(iterIdx);
    file_write_timer.stop();
    std::cout << "Write time:    " << file_write_timer.get_timing() << "\n"
              << "Read time:     " << file_read_timer.get_timing() << "\n";

    total_timer.stop();
    std::cout << "Total time:    " << total_timer.get_timing() << "\n\n";

    return 0;
}
