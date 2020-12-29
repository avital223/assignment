#include "minCircle.h"

/**
 * the method calculate the distance between two points
 * @param p1 - first point
 * @param p2 - second point
 * @return the distance between the points
 */
float distance(const Point &p1, const Point &p2) {
    return (float) sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/**
 * check if the point is in the circle
 * @param point - get the new point value
 * @param circle - the circle that we check if the point is in it
 * @return true if the point is in the circle, otherwise return false
 */
bool isPointInCircle(const Point &point, const Circle &circle) {
    float dis = distance(circle.center, point);
    return dis <= circle.radius;
}

/**
 * create the circle from two points
 * @param p1 first point
 * @param p2 second point
 * @return the new circle
 */
Circle createCircleFromTwoPoints(const Point &p1, const Point &p2) {
    float diameter = distance(p1, p2);
    auto radius = (float) (diameter / 2.0);
    //the average between the points
    Point center = {(float) ((p1.x + p2.x) / 2.0), (float) ((p1.y + p2.y) / 2.0)};
    return {center, radius};
}

/**
 * create the circle from three points.
 * we have used the algorithm from the web: http://www.ambrsoft.com/trigocalc/circle3d.htm
 * @param p1 first point
 * @param p2 second point
 * @param p3 third point
 * @return the new circle
 */
Circle createMinCircleFromThreePoints(const Point &p1, const Point &p2, const Point &p3) {
    // check if there is a point that placed between the other two points
    Circle twoPointsCircle = createCircleFromTwoPoints(p1, p2);
    if (isPointInCircle(p3, twoPointsCircle)) {
        return twoPointsCircle;
    }
    twoPointsCircle = createCircleFromTwoPoints(p2, p3);
    if (isPointInCircle(p1, twoPointsCircle)) {
        return twoPointsCircle;
    }
    twoPointsCircle = createCircleFromTwoPoints(p1, p3);
    if (isPointInCircle(p2, twoPointsCircle)) {
        return twoPointsCircle;
    }
    //using three equations with three variables x of center, y of center and radius.
    //the calculations below finding the variables.
    auto p1XInPowTwo = pow(p1.x, 2);
    auto p1YInPowTwo = pow(p1.y, 2);
    auto p2XInPowTwo = pow(p2.x, 2);
    auto p2YInPowTwo = pow(p2.y, 2);
    auto p3XInPowTwo = pow(p3.x, 2);
    auto p3YInPowTwo = pow(p3.y, 2);
    auto upperX = (p1XInPowTwo + p1YInPowTwo) * (p2.y - p3.y) + (p2XInPowTwo + p2YInPowTwo) * (p3.y - p1.y) +
                  (p3XInPowTwo + p3YInPowTwo) * (p1.y - p2.y);
    auto lowerX = 2 * (p1.x * (p2.y - p3.y) - p1.y * (p2.x - p3.x) + p2.x * p3.y - p3.x * p2.y);
    auto upperY = (p1YInPowTwo + p1XInPowTwo) * (p3.x - p2.x) + (p2XInPowTwo + p2YInPowTwo) * (p1.x - p3.x) +
                  (p3XInPowTwo + p3YInPowTwo) * (p2.x - p1.x);
    auto lowerY = lowerX;
    auto y = (float) (upperY / lowerY);
    auto x = (float) (upperX / lowerX);
    Point center = {x, y};
    float radius = distance(p1, center);
    Circle circle = {center, radius};
    return circle;
}

/**
 * The method finding the minimum circle from other circles by the algorithm of welzl
 * @param points - the points array that we check from if the points are in the circle
 * @param size - the size of points array
 * @param R - points array of the points that are on the orbit of the circle
 * @param sizeR - the size of the R array
 * @return the min circle that circulate the points
 */
Circle findMinCircleWelzlAlg(Point **points, size_t size, Point **R, size_t sizeR) {
    //the break point of the recursion
    if (size == 0) {
        if (sizeR == 0)
            return {{0, 0}, 0};
        else if (sizeR == 1)
            return {*R[0], 0};
        else if (sizeR == 2)
            return createCircleFromTwoPoints(*R[1], *R[0]);
    }
    if (sizeR == 3)
        return createMinCircleFromThreePoints(*R[0], *R[1], *R[2]);
    Circle minCircle = findMinCircleWelzlAlg(points, size - 1, R, sizeR);
    if (isPointInCircle(*points[size - 1], minCircle)) {
        return minCircle;
    }
    R[sizeR] = points[size - 1];
    minCircle = findMinCircleWelzlAlg(points, size - 1, R, sizeR + 1);
    return minCircle;
}

/**
 * the method finding the min circle
 * @param points - the points array that we check from if the points are in the circle
 * @param size - the size of points array
 * @return the minimum circle
 */
Circle findMinCircle(Point **points, size_t size) {
    Point **copyPoints;
    copyPoints = points;
    Point *R[3];
    Circle circle = findMinCircleWelzlAlg(copyPoints, size, R, 0);
    return circle;
}