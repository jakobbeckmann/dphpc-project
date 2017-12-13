//
// Created by Polly Lilly on 21.11.17.
//

#include "GrahamScanAlgorithm.h"
#include "FileWriter.h"

/**
    Retruns list of points belonging to a convex hull after running Graham Scan algorithm.
 */
 std::vector<Point> GrahamScanAlgorithm::run(const std::vector<Point>& points_, int subsetIdx, size_t) {
     std::vector<Point> points = points_;

 #if WRITE_DEBUG
     FileWriter grahamWriter;
     grahamWriter.setGrahamSubsetIdx(subsetIdx);
     grahamWriter.setBaseName("out_graham_sub");
     grahamWriter.updateFileName();
     grahamWriter.cleanOutputFile();
 #endif

     if (points.size() <= 1) {
         return points;
     }

     int bottomLeftPointIdx = findLowestLeftmostPointIndex(points);
     std::swap(points[bottomLeftPointIdx], points[0]);
     const Point& startingPoint = points[0];

     // Returns the point with lowest polar angle w.r.t startingPoint
     std::stable_sort(points.begin()+1, points.end(), [&startingPoint](const Point& p1, const Point& p2) {
         int orient = getOrientation(startingPoint, p1, p2);
         if(orient == COLLINEAR) {
             return getDistance(startingPoint, p1) <= getDistance(startingPoint, p2);
         }
         return orient != CLOCKWISE;
     });

 #if WRITE_DEBUG
     const std::string subSortedName = std::string("../Output/out_sorted_sub_") + std::to_string(subsetIdx) + ".dat";
     FileWriter::writePointsToFile(points, subSortedName, true);
 #endif

     // Find the hull
     std::vector<Point> hull;
     std::vector<int> idxStack;

     hull.push_back(startingPoint);  // Adding first point to hull.
     idxStack.push_back(0);

     // Graham algorithm core
     for (int idx = 1; idx < points.size(); idx++) {

         Point base = points[idx];
         int last_idx = hull.size() - 1;

         while (hull.size() > 1 && getOrientation(hull[last_idx - 1], hull[last_idx], base) != ANTICLOCKWISE) {
 #if WRITE_DEBUG
             grahamWriter.writeGraham(hull[last_idx-1], hull[last_idx], base, 0, hull[last_idx], CLOCKWISE);
 #endif

             hull.pop_back();
             idxStack.pop_back();

             last_idx = hull.size() - 1;

         }

         if (hull.empty() || hull[last_idx] != base) {
 #if WRITE_DEBUG
             grahamWriter.writeGraham(hull[last_idx-1], hull[last_idx], base, 1, base, ANTICLOCKWISE);
 #endif
             hull.push_back(base);
             idxStack.push_back(idx);
         }
     }

 #if WRITE_DEBUG
     const std::string subHullName = std::string("../Output/out_hull_points_") + std::to_string(subsetIdx) + ".dat";
     FileWriter::writePointsToFile(hull, subHullName, true);
 #endif

     return hull;
 }
