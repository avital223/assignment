#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    const auto keyValues = ts.getKeyValues();
    float pears, maxPears;
    unsigned int first = 0, second = 1, maxSecond;
    for (auto itKeys = keyValues.begin(); itKeys != keyValues.end(); itKeys++) {
        for (auto itComp = itKeys + 1; itComp != keyValues.end(); itComp++) {
            pears = pearson(ts.getValuesFromKey(*itKeys).data(),
                            ts.getValuesFromKey(*itComp).data(), ts.getValuesFromKey(*itKeys).size());
            if (abs(maxPears) < abs(pears)) {
                maxPears = pears;
                maxSecond = second;
            }
            second++;
        }
        if (maxPears >= 0.9 || maxPears <= -0.9) {
            cf.push_back(getCorrelationFeatures(*itKeys, keyValues.at(maxSecond),
                                                ts.getValuesFromKey(*itKeys).data(),
                                                ts.getValuesFromKey(keyValues.at(maxSecond)).data(),
                                                ts.getValuesFromKey(*itKeys).size(), maxPears));
        }
        second = 1 + ++first;
        maxPears = 0;
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    for_each(this->cf.begin(), this->cf.end(), [ts, this](correlatedFeatures corFeatures) {
        unsigned int timeStep = 1;
        auto vecA = ts.getValuesFromKey(corFeatures.feature1);
        auto vecB = ts.getValuesFromKey(corFeatures.feature2);
        vector<float>::const_iterator iterator = vecB.begin();
        for_each(vecA.begin(), vecA.end(), [&iterator, corFeatures, this, &timeStep](auto &firstVal) {
            Point point = Point(firstVal, *iterator);
            float devP = dev(point, corFeatures.lin_reg);
            if (abs(devP) > corFeatures.threshold) {
                anReport.push_back(AnomalyReport{corFeatures.feature1 + "-" + corFeatures.feature2,
                                                 timeStep});
            }
            iterator++;
            timeStep++;
        });
    });
    return anReport;
}

const correlatedFeatures
SimpleAnomalyDetector::getCorrelationFeatures(const string &first, const string &second, const float *x, const float *y,
                                              int size,
                                              const float pearson) const {
    Point *points[size];
    for (int i = 0; i < size; i++) {
        points[i] = new Point(x[i], y[i]);
    }
    Line linear = linear_reg(points, size);
    float maxDis = 0, resault;
    for (int i = 0; i < size; i++) {
        resault = dev(*points[i], linear);
        maxDis = maxDis < resault ? resault : maxDis;
        delete points[i];
    }
    return correlatedFeatures{first, second, pearson, linear, static_cast<float>(maxDis * 1.1)};
}


