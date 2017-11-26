/**
    Main function for quickhill 2d algorithm implementation.
 */

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include "../../src/Point.h"
#include "Quickhull.hpp"

int main(int argc, char const *argv[]) {
    /*
     * Input reader, remove in final build.
     * Current used file located in quickhull/static/bird_points.dat
     * Checked hull file: quickhull/static/out_hull_points.dat
     */
    std::vector<Point> points;
    std::ifstream infile("static/bird_points.dat");
    float a, b;
    while(infile >> a >> b) {
        points.push_back(Point(a, b));
    }
    infile.close();

    std::vector<int> result;

    
    Quickhull::run(points, result);

    /*
     * Output writer, remove in final build.
     * Checked hull file: quickhull/static/out_hull_points.dat
     */
    for(int idx: result){
        points[idx].print();
    }
}
