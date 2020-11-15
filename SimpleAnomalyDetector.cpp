
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    // TODO Auto-generated destructor stubconst
    const vector<string> &keyValues = ts.getKeyValues();
    vector<string>::const_iterator itKeys, itComp;
    float pears, maxPears;
    unsigned int first = 0, second = 0, maxSecond;
    for (itKeys = keyValues.begin(); itKeys != keyValues.end(); itKeys++) {
        for (itComp = itKeys, itComp++; itComp != keyValues.end(); itComp++) {
            pears = pearson((ts.getValuesFromKey(*itKeys)).begin(),
                            (float *) &ts.getValuesFromKey(*itComp), ts.getValuesFromKey(*itKeys).size());
            if (maxPears < pears) {
                maxPears = pears;
                maxSecond = second;
            }
        }
        if (maxPears < 0.9 && maxPears > -0.9) {
            cf.push_back(getCorrelationFeatures(*itKeys, keyValues.at(maxSecond),
                                                (float *) &(ts.getValuesFromKey(*itKeys)),
                                                (float *) &ts.getValuesFromKey(keyValues.at(maxSecond)),
                                                ts.getValuesFromKey(*itKeys).size(), maxPears));
        }
        second = 1 + ++first;
        maxPears = 0;
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    // TODO Auto-generated destructor stub
}

const correlatedFeatures
SimpleAnomalyDetector::getCorrelationFeatures(const string &first, const string &second, float *x, float *y, int size,
                                              const float pearson) const {
    Point **points;
    *points = (Point *) malloc(size * sizeof(Point));
    for (int i = 0; i < size; i++) {
        points[i] = new Point(x[i], y[i]);
    }
    Line linear = linear_reg(points, size);
    float threshold, maxDis = 0, resault;
    for (int i = 0; i < size; i++) {
        resault = dev(*(points[i]), linear);
        maxDis = maxDis < resault ? resault : maxDis;
        delete points[i];
    }
    delete points;
    return correlatedFeatures{first, second, pearson, linear, threshold};
}


