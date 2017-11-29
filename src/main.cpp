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


int main(int argc, char const *argv[]) {
    #ifdef WRITE_DEBUG
        std::cout << "Running in DEBUG mode - writing ALL output files." << "\n";
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
        ChanAlgorithm chan;

        timer.start();
        result = chan.run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else if (algorithm == "chan_merge_var") {
        ChanAlgorithm2Merge chan;
        timer.start();
        result = chan.run(points, numberOfCores, numberOfCores /* TODO number of parts*/);
        timer.stop();

    } else  if (algorithm == "graham") {
        GrahamScanAlgorithm graham;
        timer.start();
        result = graham.run(points);
        timer.stop();

    } else  if (algorithm == "jarvis") {
        JarvisAlgorithm jarvis;
        timer.start();
        result = jarvis.run(points);
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

    std::cout << "\n\n=========Result=========" << "\n";
    std::cout << "Algorithm:     " << algorithm << "\n";
    std::cout << "Input size:    " << points.size() << "\n";
    std::cout << "N hull points: " << result.size() << "\n";
    std::cout << "Iteration:     " << iterIdx << "\n";
    std::cout << "Time used:     " << timer.get_timing() << "\n";

    file_write_timer.start();
    std::stringstream fileName;
    fileName << "hull_points_" << iterIdx << ".dat";
    FileWriter::writePointsToFile(result, fileName.str(), true);

    timer.write_to_file(iterIdx);
    file_write_timer.stop();
    std::cout << "Write time:    " << file_write_timer.get_timing() << "\n";
    std::cout << "Read time:     " << file_read_timer.get_timing() << "\n";

    total_timer.stop();
    std::cout << "Total time:    " << total_timer.get_timing() << "\n";
    std::cout << std::endl;

    return 0;
}
