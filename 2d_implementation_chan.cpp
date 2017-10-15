/*
2d_implementation_chan.cpp
08-10-2017

@Description: 2D implementation of Chan's algorithm. Note that we do not consider a point to be part of the hull if it
is the middle point of 3 collinear points. Hence no three points in a hull (either global or local/Graham) will ever
be collinear.  The reason for this design choice is that colinear points only add computation for hull mergers.
They do output a different polygon but the shape of this output and the one with all collinear points is exacly
the same.
Note that this adds the restrinction that we require the level of parallelism to be less than a third of the points.
Otherwise at least one subset of the points being analysed by Graham's algorithm will of size 2 and as we do not consider
collinear points to be part of hulls, Graham's scan returns an empty hull.
By extension we assume that the points have general position and hence no subset of points being analysed by Graham's
algorithm contains only collinear points.

@Author: Jakob Beckmann
*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <utility>

#define RIGHT_TURN -1
#define LEFT_TURN 1
#define COLLINEAR 0
#define EPSILON 0.0001


/*
    Class handling 2D point objects.
*/
class Point {
public:
    double x, y;

    // Constructor
    Point(double x=0, double y=0) {
        this->x = x;
        this->y = y;
    }

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
}p0;

/*
    Returns the orientation of the angle between three points.
    Returns -1: Right turn
             1: Left turn
             0: Collinear
    @param p1: first point
    @param p2: second point
    @param p3: third point
*/
int orientation(Point p1, Point p2, Point p3) {
    double cross_product = (p1.y - p2.y) * (p3.x - p2.x) - (p1.x - p2.x) * (p3.y - p2.y);
    if(fabs(cross_product) < EPSILON) return COLLINEAR;
    return (cross_product > 0)? LEFT_TURN: RIGHT_TURN;
}

/*
    Returns square of the distance between two 2D points.
    @param point1: first point.
    @param point2: second point.
*/
double distance(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}


/*
    Function used when sorting using qsort().
    Returns the point with lowest polar angle w.r.t p0(0, 0) and the x-axis.
    @param vpp1: pointer to first point
    @param vpp2: pointer to second point
*/
int compare(const void* vpp1, const void* vpp2) {
    Point* pp1 = (Point*) vpp1;
    Point* pp2 = (Point*) vpp2;
    int orient = orientation(p0, *pp1, *pp2);
    if(orient == COLLINEAR) {
        return (distance(p0, *pp1) <= distance(p0, *pp2))? 1: -1;
    }
    return (orient == RIGHT_TURN)? 1: -1;
}

/*
    Finds the clockwise hull of a vector of points. If the base does not lie inside the hull, it is added to the hull.
    In essence, this extends the hull by the base point and corrects it for convexity.
    Returns a vector of points.
    @param points: vector of points
    @param base: point
*/
std::vector<Point> hull_check(std::vector<Point>& points, Point base) {
    int last_idx = points.size() - 1;
    while(points.size() > 1 && orientation(points[last_idx - 1], points[last_idx], base) != LEFT_TURN) {
        points.pop_back();
        last_idx = points.size() - 1;
    }
    if(points.size() == 0 || points[last_idx] != base) {
        points.push_back(base);
    }
    return points;
}


/*
    Performs a Graham Scan on input vector of points.
    Returns a vector of points contained in the convex hull of the input.
    @param points: vector of points
*/
std::vector<Point> graham_scan(std::vector<Point>& points) {
    if(points.size() <= 1) {
        return points;
    }
    // Sort the points based on polar angles w.r.t. p0
    std::qsort(&points[0], points.size(), sizeof(Point), compare);

    // Find the hull
    std::vector<Point> hull;
    for(int idx = 0; idx < points.size(); idx++) {
        hull = hull_check(hull, points[idx]);
    }
    // Check closure of hull
    hull = hull_check(hull, points[0]);
    hull.pop_back();

    return hull;
}


/*
    Returns the index of the point in the list that sits such that each other point
    in the list is on the left of the line from said point to the base point given in
    the arguments.
    @param points: vector of points forming a convex polygon.
    @param base: base point
*/
int tangent_idx(std::vector<Point> points, Point base) {
    int lower_bound = 0;
    int upper_bound = points.size();

    // Find orientation of turns for points right before and after the lower bound.
    int lb_turn_before = orientation(base, points[0], points[points.size() - 1]);
    int lb_turn_after = orientation(base, points[0], points[1]);

    // Check if first point is the point lying to the right of all other points.
    if(lb_turn_before != RIGHT_TURN && lb_turn_after == LEFT_TURN) {
        return 0;
    }

    // First point is not the right most point.
    while(lower_bound < upper_bound) {
        // Find index of point in between the two bounds.
        int mid = (upper_bound + lower_bound) / 2;

        // Compute its turns.
        int mid_turn_before = orientation(base, points[mid], points[(mid - 1) % points.size()]);
        int mid_turn_after = orientation(base, points[mid], points[(mid + 1) % points.size()]);

        // Check in which direction the next cut should be based on the position relative to
        // the lower_bound point.
        int cut_direction = orientation(base, points[lower_bound], points[mid]);

        if(mid_turn_before != RIGHT_TURN && mid_turn_after == LEFT_TURN) {
            // All points lie to the right of 'mid'
            return mid;
        } else if((cut_direction != RIGHT_TURN && lb_turn_after != LEFT_TURN) ||
                  (cut_direction == RIGHT_TURN && mid_turn_before == RIGHT_TURN)) {
            // The leftmost point lies to the right of the cut (line between 'lower_bound' and 'mid')
            upper_bound = mid;
        } else {
            // The leftmost point lies to the left of the cut.
            lower_bound = mid + 1;
        }
        lb_turn_after = orientation(base, points[lower_bound], points[(lower_bound + 1) % points.size()]);
    }
    return lower_bound;
}


/*
    Returns a pair representing the hull number and point index inside that hull. The represented
    point is the point with lowest y coordinate across all hulls.
    @param hulls: vector of hulls (vectors of points)
*/
std::pair<int, int> lowest_point(std::vector<std::vector<Point> > hulls) {
    int hull = 0, point = 0;
    double lowest_y = hulls[0][0].y;
    for(int hull_idx = 0; hull_idx < hulls.size(); hull_idx++) {
        for(int point_idx = 0; point_idx < hulls[hull_idx].size(); point_idx++) {
            if(hulls[hull_idx][point_idx].y < lowest_y) {
                hull = hull_idx;
                point = point_idx;
            }
        }
    }
    return std::make_pair(hull, point);
}


/*
    Returns the hull-point pair that will be used as the next point for the hull merge.
    @param hulls: vector of hulls (vectors of points)
    @param base_pair: base point (the last added point from the hull merge)
*/
std::pair<int, int> next_merge_point(std::vector<std::vector<Point> > hulls, std::pair<int, int> base_pair) {
    int hull = 0, point = 0;
    Point base = hulls[base_pair.first][base_pair.second];
    // Select next point on the same hull as the next point for the merge
    std::pair<int, int> result = std::make_pair(base_pair.first, (base_pair.second + 1) % hulls[base_pair.first].size());
    for(int hull_idx = 0; hull_idx < hulls.size(); hull_idx++) {
        if(hull_idx != base_pair.first) {
            int candidate_idx = tangent_idx(hulls[hull_idx], base);
            Point previous = hulls[result.first][result.second];
            Point candidate = hulls[hull_idx][candidate_idx];
            int linearity = orientation(base, previous, candidate);
            if(linearity == RIGHT_TURN || (linearity == COLLINEAR && distance(base, previous) < distance(base, candidate))) {
                result = std::make_pair(hull_idx, candidate_idx);
            }
        }
    }
    return result;
}



/*
    Returns the 2D convex hull of the points given in the argument. The parallelism index determines how many subsets of
    points should be analysed in parallel using Graham's scan.
    @param points: vector of points the be analysed
    @param parallel_idx: parallelism index determining the amount of parallel computation
*/
std::vector<Point> chan(std::vector<Point> points, int parallel_idx) {
    std::vector<std::vector<Point> > hulls;
    /*
        TODO The following sode snipped will need to be parallelised using mpi in order to achieve real parallelism.
    */
    for(int idx = 0; idx < parallel_idx; idx++) {
        std::vector<Point> subset;
        for(int point_idx = idx; point_idx < points.size(); point_idx += parallel_idx) {
            subset.push_back(points[point_idx]);
        }
        hulls.push_back(graham_scan(subset));
    }
    /*
        TODO END
    */
    std::pair<int, int> next_point = lowest_point(hulls);
    std::vector<Point> result;
    result.push_back(hulls[next_point.first][next_point.second]);
    do {
        next_point = next_merge_point(hulls, next_point);
        result.push_back(hulls[next_point.first][next_point.second]);
    } while(result[0] != result[result.size() - 1]);
    result.pop_back();
    return result;
}




int main(int argc, char const *argv[]) {
    int POINT_COUNT = 0, PARALLELISM_IDX = 1;
    double x = 0, y = 0;

    std::cout << "Please enter the parallelism index:" << std::endl;
    std::cin >> PARALLELISM_IDX;
    if(PARALLELISM_IDX < 1) return -1;

    std::cout << "Please enter the total number of points:" << std::endl;
    std::cin >> POINT_COUNT;
    if(POINT_COUNT < PARALLELISM_IDX * 3) {
        std::cout << "Please need more points or less parallelism." << std::endl;
        return -1;
    }

    std::vector<Point> points;
    for(int idx = 0; idx < POINT_COUNT; idx++) {
        std::cout << "Coordinates of point " << idx + 1 << ": ";
        std::cin >> x >> y;
        points.push_back(Point(x, y));
    }

    // std::vector<Point> result = graham_scan(points);
    std::vector<Point> result = chan(points, PARALLELISM_IDX);
    std::cout << "=========Result=========" << std::endl;
    for(Point point: result) {
        std::cout << point << " ";
    }
    std::cout << std::endl;
    return 0;
}
