#pragma once

#include "FileWriter.h"
#include "utility.h"

class ChanAlgorithm {
private:
    Point startingPoint;

    int lowestAngleSort(const Point& pp1, const Point& pp2);
    int findTangentIndex(vector<Point> points, Point base);
    //vector<Point> checkHull(vector<Point>& points, Point base, FileWriter& fileWriter);
    vector<Point> grahamScan(vector<Point>& points, int subsetIdx);
    pair<int, int> findLowestPoint(vector<vector<Point> > hulls);
    pair<int, int> findNextMergePoint(vector<vector<Point> > hulls, pair<int, int> base_pair);
    /*
    struct doCompare
    {
        explicit doCompare( const ChanAlgorithm& chan ) : m_chan(chan) { } // only if you really need the object state
        const ChanAlgorithm& m_chan;

        int operator()(const Point& p1, const Point& p2 )
        {
            int orient = getOrientation(m_chan.startingPoint, p1, p2);
            if(orient == COLLINEAR) {
                return (getDistance(m_chan.startingPoint, p1) <= getDistance(m_chan.startingPoint, p2))? -1: 1;
            }
            return (orient == CLOCKWISE)? 1: -1;
        }
    };
     */

public:
    vector<Point> run(vector<Point> points, int parallel_idx);

};

