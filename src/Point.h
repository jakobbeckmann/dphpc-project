/**
    Point class representing a 2D point and some operators.
 */

#ifndef DPHPC_PROJECT_POINT_H
#define DPHPC_PROJECT_POINT_H

#include <cmath>
#include <iostream>

#include "constants.h"

/**
    Class handling 2D point objects.
*/

class Point {
public:
    double x, y;

    // Constructor
    explicit Point(double x=0, double y=0)
     : x(x), y(y)
    {}

    // Overload operators
    friend bool operator== (const Point& p1, const Point& p2) {
        return (fabs(p1.x - p2.x) < EPSILON) && (fabs(p1.y - p2.y) < EPSILON);
    }
    friend bool operator!= (const Point& p1, const Point& p2) {
        return (fabs(p1.x - p2.x) > EPSILON) || (fabs(p1.y - p2.y) > EPSILON);
    }
    friend std::ostream& operator<< (std::ostream& output, const Point& point) {
        output << "(" << point.x << ", " << point.y << ")";
        return output;
    }
};


#endif //DPHPC_PROJECT_POINT_H
