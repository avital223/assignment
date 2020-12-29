/*
 * SimpleAnomaityDetector.cpp
 *
 * Author: Avital Livshitz 318850575
 */
#include "SimpleAnomalyDetector.h"

//Using TimeSeries, this function finds which features have strong correlation, and saves them.
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    const auto &features = ts.getFeatures();
    const auto end = features.end();
    //to each feature check the feature with the strongest correlation.
    auto itFeat = features.begin();
    for (int i = 0; i < features.size(); i++) {
        float pears, maxPears = 0;
        int second = i + 1, maxSecond = 0;
        for (auto itComp = itFeat + 1; itComp != end; itComp++) {
            pears = pearson(ts.getDataFromFeature(*itFeat).data(),
                            ts.getDataFromFeature(*itComp).data(), ts.getDataFromFeature(*itFeat).size());
            if (fabs(maxPears) < fabs(pears)) {
                //saves the better correlated feature
                maxPears = pears;
                maxSecond = second;
            }
            second++;
        }
        maxCorrelation maxCorr = maxCorrelation{i, maxSecond, maxPears};
        addCorrelationIfNeeded(ts, maxCorr);
        itFeat++;
    }
}


//According to what features the SimpleAnormalityDetector knows are correlated,
//the function return a vector of anomaly features correlation.
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anReport;
    for_each(this->cf.begin(), this->cf.end(), [&ts, this, &anReport](const correlatedFeatures &corFeatures) {
        unsigned int timeStep = 1;
        const auto &vecFeature1 = ts.getDataFromFeature(corFeatures.feature1);
        const auto &vecFeature2 = ts.getDataFromFeature(corFeatures.feature2);
        auto iterator2 = vecFeature2.begin(), iterator1 = vecFeature1.begin();
        for (; iterator1 != vecFeature1.end() && iterator2 != vecFeature2.end(); iterator1++) {
            Point point = Point(*iterator1, *iterator2);
            AnomalyReport anomalyReport = checkAnomalityForPoint(point, corFeatures, timeStep);
            if (anomalyReport.description != "nan") {
                anReport.push_back(anomalyReport);
            }
            timeStep++;
            iterator2++;
        }
    });
    return anReport;
}

//This Function calculates all the information from the 2 features and returns
//a correlatedFeatures that saves all this information.
correlatedFeatures
SimpleAnomalyDetector::getCorrelationFeatures(maxCorrelation &maxCorr, const TimeSeries &ts) {

    const auto &secFeat = ts.getFeatureNameFromPos(maxCorr.second);
    const auto &firstFeat = ts.getFeatureNameFromPos(maxCorr.first);
    const float *x = ts.getDataFromFeature(firstFeat).data();
    const float *y = ts.getDataFromFeature(secFeat).data();
    int size = ts.getDataFromFeature(firstFeat).size();
    Point *points[size];
    for (int i = 0; i < size; i++) {
        points[i] = new Point(x[i], y[i]);
    }
    Line linear = linear_reg(points, size);
    float maxDis = 0.0, result = 0.0;
    for (int i = 0; i < size; i++) {
        result = dev(*points[i], linear);
        if (maxDis < result)
            maxDis = result;

        delete points[i];
    }
    return correlatedFeatures{ts.getFeatureNameFromPos(maxCorr.first),
                              ts.getFeatureNameFromPos(maxCorr.second), maxCorr.pearson, linear, maxDis * 1.1f,
                              {{0, 0}, 0}};
}

//checks if the point is out of the normal of the correlated feature.
AnomalyReport
SimpleAnomalyDetector::checkAnomalityForPoint(const Point &point, const correlatedFeatures &correlatedFeatures,
                                              unsigned int timeStep) {
    float devP = dev(point, correlatedFeatures.lin_reg);
    if (devP > correlatedFeatures.threshold) {
        return {correlatedFeatures.feature1 + "-" + correlatedFeatures.feature2, timeStep};
    }
    return empty;
}

void SimpleAnomalyDetector::addCorrelationIfNeeded(const TimeSeries &ts,
                                                   maxCorrelation &maxCorrelation) {
    float pears = maxCorrelation.pearson;
    //we need to save only strong correlated features.
    if (fabs(pears) >= minPears) {
        cf.push_back(getCorrelationFeatures(maxCorrelation, ts));
    }
}

void SimpleAnomalyDetector::setMinPears(float min) {
    if (fabs(min) <= 1.0f) {
        minPears = min;
    }
}

