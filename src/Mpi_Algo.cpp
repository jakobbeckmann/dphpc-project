#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>
#include <sstream>
#include <mpi.h>

#include "timer.h"
#include "Point.h"
#include "FileWriter.h"
#include "Jarvis_Jarvis.hpp"
#include "Jarvis_Graham.hpp"
#include "Jarvis_Quickhull.hpp"
#include "Graham_Jarvis.hpp"
#include "Graham_Graham.hpp"
#include "Graham_Quickhull.hpp"
#include "Quickhull_Jarvis.hpp"
#include "Quickhull_Graham.hpp"
#include "Quickhull_Quickhull.hpp"
#include "ChanAlgorithm.h"
#include "Jarvis_BinJarvis.hpp"
#include "Quickhull_BinJarvis.hpp"

#define INPUT_MESSAGE 0
#define HULL_MESSAGE 1
#define MERGE_PROCESS_ID 0

struct algo {
    std::string name;
    std::function<std::vector<Point>(const std::vector<Point> &, int, size_t)> func;
};

static const std::vector <algo> simple_algos = {
        {"jarvis_jarvis",       Jarvis_Jarvis::run },
        {"jarvis_graham",       Jarvis_Graham::run },
        {"jarvis_quickhull",    Jarvis_Quickhull::run },
        {"graham_jarvis",       Graham_Jarvis::run },
        {"graham_graham",       Graham_Graham::run },
        {"graham_quickhull",    Graham_Quickhull::run },
        {"quickhull_jarvis",    Quickhull_Jarvis::run },
        {"quickhull_graham",    Quickhull_Graham::run },
        {"quickhull_quickhull", Quickhull_Quickhull::run },
        {"chan_normal",         ChanAlgorithm::run },
        {"jarvis_binjarvis",    Jarvis_BinJarvis::run },
        {"quickhull_binjarvis", Quickhull_BinJarvis::run }
};

int main(int argc, char *argv[]) {
    // Data to keep track on the time
    Timer timer;
    Timer file_write_timer;
    Timer file_read_timer;
    Timer total_timer;

    // Data to keep track on the execution process
    int tid, nprocesses, provided;
    int total_hulls = 0;
    int initial_points_size;
    std::vector <Point> points;

    // Configuration data
    int n_cores = atoi(argv[1]);
    auto part_size = size_t(atoi(argv[2]));
    auto numberOfCores = (size_t) n_cores;
    int iterIdx = atoi(argv[3]);
    std::string inputFile = argv[4];
    std::string algorithm = argv[5];

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &tid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocesses);

    if (tid == MERGE_PROCESS_ID) {
        total_timer.start();
    }

    // Only merge process execution: reading input points and distributing across processes
    if (tid == MERGE_PROCESS_ID) {
        file_read_timer.start();
        std::vector <Point> allPoints = readPointsFromFile(inputFile);
        file_read_timer.stop();
        initial_points_size = allPoints.size();
        timer.start();

        // Distribute points to different processes in parallel
//        omp_set_num_threads(2);
        #pragma omp parallel for
        for (int i = 0; i < nprocesses; ++i) {
            std::vector <Point> part = SplitVector(allPoints, i, nprocesses);
            if (i == MERGE_PROCESS_ID) {
                // Merge process picks his points
                points = part;
            } else {
                // Merge process sends points to other processes
                MPI_Send(part.data(), part.size(), MPI_LONG_DOUBLE, i, INPUT_MESSAGE, MPI_COMM_WORLD);
            }
        }
    } else {
        // Only non-merge processes execution: receiving points data input
        std::vector <Point> input;

        // Finding out the exact size of the received message
        MPI_Status status;
        int count;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_LONG_DOUBLE, &count);
        input.resize(count);

        // Receiving the points data
        MPI_Recv(&input[0], count, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, INPUT_MESSAGE, MPI_COMM_WORLD,
                 &status);
        points = input;
    }

    // All processes execute fully the algorithm specified
    std::vector <Point> intermediate_hull;
    std::vector<algo>::const_iterator it;
    if ((it = std::find_if(simple_algos.begin(), simple_algos.end(), [algorithm](const algo &a) {
        return algorithm == a.name;
    })) != simple_algos.end()) {
        intermediate_hull = it->func(points, numberOfCores, part_size);
    } else {
        std::cout << std::string("No such algorithm! Given ") + algorithm;
        std::exit(EXIT_FAILURE);
    }

    // Only non-merging processes execution: sending calculated hull
    if (tid != MERGE_PROCESS_ID) {
        MPI_Send(intermediate_hull.data(), intermediate_hull.size(), MPI_LONG_DOUBLE, MERGE_PROCESS_ID, HULL_MESSAGE,
                 MPI_COMM_WORLD);
    } else {
        total_hulls++;
        // Only merge process execution: receiving calculated hull
        while (total_hulls < nprocesses) {
            std::vector <Point> received_hull;

            // Finding out the exact size of the received message
            MPI_Status status;
            std::vector<int> counts(nprocesses);
            MPI_Probe(MPI_ANY_SOURCE, HULL_MESSAGE, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_LONG_DOUBLE, &counts[status.MPI_SOURCE]);
            received_hull.resize(counts[status.MPI_SOURCE]);

            // Receiving the calculated hull
            MPI_Recv(&received_hull[0], counts[status.MPI_SOURCE], MPI_LONG_DOUBLE, status.MPI_SOURCE, HULL_MESSAGE,
                     MPI_COMM_WORLD,
                     &status);

            for (Point point : received_hull) {
                intermediate_hull.push_back(point);
            }
            total_hulls++;
        }

        std::vector <Point> final_hull = it->func(intermediate_hull, numberOfCores, part_size);
        timer.stop();

        std::cout << "\n\n=========Result=========\n"
                  << "Algorithm:     " << algorithm << "\n"
                  << "Input size:    " << initial_points_size << "\n"
                  << "N hull points: " << final_hull.size() << "\n"
                  << "Iteration:     " << iterIdx << "\n"
                  << "Time used:     " << timer.get_timing() << "\n";

        file_write_timer.start();
        std::stringstream fileName;
        fileName << "hull_points_" << iterIdx << ".dat";
        FileWriter::writePointsToFile(final_hull, fileName.str(), true);

        timer.write_to_file(iterIdx);
        file_write_timer.stop();
        std::cout << "Write time:    " << file_write_timer.get_timing() << "\n"
                  << "Read time:     " << file_read_timer.get_timing() << "\n";

        total_timer.stop();
        std::cout << "Total time:    " << total_timer.get_timing() << "\n\n";

    }
    MPI_Finalize();
    return (0);
}
