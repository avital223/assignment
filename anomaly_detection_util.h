
#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

#include <iostream>

using namespace std;

float avg(const float *x, const int size);

// returns the variance of X and Y
float var(const float *x, const int size);

// returns the covariance of X and Y
float cov(const float *x, const float *y, const int size);


// returns the Pearson correlation coefficient of X and Y
float pearson(const float *x, const float *y, const int size);

class Line {
public:
    Line() : a(0), b(0) {}

    float a, b;

    Line(float a, float b) : a(a), b(b) {}

    float f(float x) {
        return a * x + b;
    }
};

class Point {
public:
    float x, y;
    Point(float x, float y) : x(x), y(y) {}


};

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size);

// returns the deviation between point p and the line
float dev(Point p, Line l);

#endif
