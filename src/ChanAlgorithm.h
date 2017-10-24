#pragma once

#include "FileWriter.h"

using namespace std;

class ChanAlgorithm {
private:

    Point origin;

    int findTangentIndex(vector<Point> points, Point base);
    vector<Point> checkHull(vector<Point>& points, Point base, int baseIdx, FileWriter& fileWriter);
    vector<Point> grahamScan(vector<Point>& points, int subsetIdx);
    pair<int, int> findLowestPoint(vector<vector<Point> > hulls);
    pair<int, int> findNextMergePoint(vector<vector<Point> > hulls, pair<int, int> base_pair);
public:
    vector<Point> run(vector<Point> points, int parallel_idx);

};

