// 31885057	31644480
// the algorithm that we used - https://en.wikipedia.org/wiki/Smallest-circle_problem
// the calculation of the variables in the method - createMinCircleFromThreePoints -
// http://www.ambrsoft.com/trigocalc/circle3d.htm

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <algorithm>
#include <cassert>
#include "anomaly_detection_util.h"
#include <cmath>

using namespace std;

// ------------ DO NOT CHANGE -----------
//class Point {
//public:
//    float x, y;
//
//    Point(float x, float y) : x(x), y(y) {}
//};

class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

/**
 * the method calculate the distance between two points
 * @param p1 - first point
 * @param p2 - second point
 * @return the distance between the points
 */
float distance(const Point &p1, const Point &p2);

/**
 * check if the point is in the circle
 * @param point - get the new point value
 * @param circle - the circle that we check if the point is in it
 * @return true if the point is in the circle, otherwise return false
 */
bool isPointInCircle(const Point &point, const Circle &circle);

/**
 * create the circle from two points
 * @param p1 first point
 * @param p2 second point
 * @return the new circle
 */
Circle createCircleFromTwoPoints(const Point &p1, const Point &p2);

/**
 * create the circle from three points.
 * we have used the algorithm from the web: http://www.ambrsoft.com/trigocalc/circle3d.htm
 * @param p1 first point
 * @param p2 second point
 * @param p3 third point
 * @return the new circle
 */
Circle createMinCircleFromThreePoints(const Point &p1, const Point &p2, const Point &p3);

/**
 * The method finding the minimum circle from other circles by the algorithm of welzl
 * @param points - the points array that we check from if the points are in the circle
 * @param size - the size of points array
 * @param R - points array of the points that are on the orbit of the circle
 * @param sizeR - the size of the R array
 * @return the min circle that circulate the points
 */
Circle findMinCircleWelzlAlg(Point **points, size_t size, Point **R, size_t sizeR) ;

/**
 * the method finding the min circle
 * @param points - the points array that we check from if the points are in the circle
 * @param size - the size of points array
 * @return the minimum circle
 */
Circle findMinCircle(Point **points, size_t size);


#endif /* MINCIRCLE_H_ */
