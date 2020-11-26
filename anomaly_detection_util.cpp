/*
 * animaly_detection_util.cpp
 *
 * Author: Avital Livshitz 318850575
 */

#include <math.h>
#include "anomaly_detection_util.h"

float avg(const float *x, const int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += *(x + i);
    }
    return sum / (float) size;
}

// returns the variance of X and Y
float var(const float *x, const int size) {
    float x2[size];
    for (int i = 0; i < size; i++) {
        *(x2 + i) = pow(*(x + i), 2);
    }
    float var = avg(x2, size) - (float) pow(avg(x, size), 2);
    return var;
}

// returns the covariance of X and Y
float cov(const float *x, const float *y, const int size) {
    float avX = avg(x, size);
    float avY = avg(y, size);
    float arr[size];
    for (int i = 0; i < size; i++) {
        *(arr + i) = (*(x + i) - avX) * (*(y + i) - avY);
    }
    float average = avg(arr, size);
    return average;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(const float *x, const float *y, const int size) {
    return cov(x, y, size) / (sqrt(var(x, size) * var(y, size)));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size) {
    float *x = new float[size];
    float *y = new float[size];
    for (int i = 0; i < size; i++) {
        *(x + i) = (*(points + i))->x;
        *(y + i) = (*(points + i))->y;
    }
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - a * avg(x, size);
    delete[]x;
    delete[]y;
    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
    Line line = linear_reg(points, size);
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    float x = p.x, y = p.y;
    return fabs(y - (l.a * x + l.b));
}





