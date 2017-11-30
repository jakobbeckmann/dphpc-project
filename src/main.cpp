/**
 *
 * Main function to call different algorithms.
 *
 */

#include <cstdio>
#include <functional>
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

struct algo {
    std::string name;
	std::function<std::vector<Point>(const std::vector<Point>&, int, size_t)> func;
};

// TODO add that to the class, instead of having it here.
static std::vector<Point> plain_quickhull(const std::vector<Point>& points, int parallel_idx, size_t parts)
{
    std::vector<Point> result;
    std::vector<int> result_idxs;
    Quickhull::run(points, result_idxs);
    for(int idx: result_idxs) {
        result.push_back(points[idx]);
    }
    return result;
}

static const std::vector<algo> algos = {
    { "chan_normal", ChanAlgorithm::run },
    { "chan_merge_var", ChanAlgorithm2Merge::run },
    { "graham", GrahamScanAlgorithm::run }, // Ignores all parameters apart from points.
    { "jarvis", JarvisAlgorithm::run }, // Ignores all parameters apart from points.
    { "jarvis_jarvis", Jarvis_Jarvis::run },
    { "jarvis_binjarvis", Jarvis_BinJarvis::run },
    { "jarvis_graham", Jarvis_Graham::run },
    { "jarvis_quickhull", Jarvis_Quickhull::run },
    { "graham_jarvis", Graham_Jarvis::run },
    { "graham_graham", Graham_Graham::run },
    { "graham_quickhull", Graham_Quickhull::run },
    { "quickhull_binjarvis", Quickhull_BinJarvis::run },
    { "quickhull_jarvis", Quickhull_Jarvis::run },
    { "quickhull_graham", Quickhull_Graham::run },
    { "quickhull_quickhull", Quickhull_Quickhull::run },
    { "quickhull", plain_quickhull }
};

int main(int argc, char const *argv[]) {
    #ifdef WRITE_DEBUG
        std::cout << "Running in DEBUG mode - writing ALL output files.\n";
    #endif

    if (argc < 6) {
        std::cout << "Usage: " << " n_cores part_size input_file algorithm iter_idx\n"
                     "  algorithm := ";
        for (const algo& a : algos) {
            std::cout << a.name << " | ";
        }
        std::cout << "\n";

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

    std::vector<algo>::const_iterator it;
    if ((it = std::find_if(algos.begin(), algos.end(), [algorithm](const algo& a) {
            return algorithm == a.name;
        })) != algos.end()) {
        timer.start();
        result = it->func(points, numberOfCores, part_size);
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
