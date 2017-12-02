#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>
#include <sstream>
#include "stdio.h"
#include <stdlib.h>

#include <mpi.h>
#include "Graham_Jarvis.hpp"

#define INPUT_MESSAGE 0
#define HULL_MESSAGE 1
#define MERGE_PROCESS_ID 0

int main(int argc, char *argv[]) {
    // Data to keep track on the execution process
    int tid, nprocesses, provided;
    int total_hulls = 0;
//  int current_hulls = 0;

    // Configuration data
    int parallel_idx = 10;
    int parts = 10;
    const std::string inputFile = "bird_points";
    std::vector<Point> points = readPointsFromFile(inputFile);

    // MPI execution started
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &tid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocesses);

    // Only merge process execution
    if (tid == MERGE_PROCESS_ID) {
        std::vector<Point> allPoints = points;
        // Distribute points to different processes in parallel
        #pragma omp parallel for
        for (int i = 0; i < nprocesses; ++i) {
            std::vector<Point> part = SplitVector(allPoints, i, nprocesses);
            if (i == 0) {
                // Merge process picks his points
                points = part;
            } else {
                // Merge process sends points to other processes
                MPI_Send(part.data(), part.size(), MPI_LONG_DOUBLE, i, INPUT_MESSAGE, MPI_COMM_WORLD);
            }
        }
    } else {
        // Non-merge processes only execution: receiving input
        std::vector<Point> input;

        MPI_Status status;
        int count = 0;

        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        // When probe returns, the status object has the size and other
        // attributes of the incoming message. Get the message size
        MPI_Get_count(&status, MPI_LONG_DOUBLE, &count);

        // Allocate a buffer to hold the incoming numbers
        input.resize(count);

        MPI_Recv(&input[0], count, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, INPUT_MESSAGE, MPI_COMM_WORLD,
                 &status);

        points = input;
    }


    printf("Points received %d for %d\n", points.size(), tid);

    // All processes run this
    std::vector<Point> intermediate_hull = Graham_Jarvis::run(points, parallel_idx, parts);
    total_hulls++;

    printf("Points calculated %d by %d\n", intermediate_hull.size(), tid);

    // Non-merging processes only execution: sending calculated hull
    if (tid != MERGE_PROCESS_ID) {
        printf("Points size to sent %d by %d\n", intermediate_hull.size(), tid);
        MPI_Send(intermediate_hull.data(), intermediate_hull.size(), MPI_LONG_DOUBLE, MERGE_PROCESS_ID, HULL_MESSAGE, MPI_COMM_WORLD);
    } else {
        // Only merge process execution: receiving calculated hull
        while (total_hulls < nprocesses) {
            std::vector <Point> received_hull;

            MPI_Status status;
            std::vector<int> counts(nprocesses);

            MPI_Probe(MPI_ANY_SOURCE, HULL_MESSAGE, MPI_COMM_WORLD, &status);

            // Find the size of received hull in advance
            MPI_Get_count(&status, MPI_LONG_DOUBLE, &counts[status.MPI_SOURCE]);

            received_hull.resize(counts[status.MPI_SOURCE]);

            MPI_Recv(&received_hull[0], counts[status.MPI_SOURCE], MPI_LONG_DOUBLE, status.MPI_SOURCE, HULL_MESSAGE, MPI_COMM_WORLD,
                     &status);

            printf("Size of received hull  %d from %d\n",
                   received_hull.size(), status.MPI_SOURCE);

//            current_hulls++;

            // If there are at least 2 hulls: merge
//            if (current_hulls == 1) {
                for (Point point : received_hull) {
                    intermediate_hull.push_back(point);
                }
//                intermediate_hull = JarvisAlgorithm::run(intermediate_hull, 0/*unused*/, 0/*unused*/);
//                current_hulls = 0;
//            }
            total_hulls++;
        }

        printf("Intermediate hull size  %d\n",
               intermediate_hull.size());

        std::vector <Point> final_hull = JarvisAlgorithm::run(intermediate_hull, 0/*unused*/, 0/*unused*/); //intermediate_hull;

        for(Point point: final_hull) {
            point.print();
        }
    }
    MPI_Finalize();
    return(0);
}
