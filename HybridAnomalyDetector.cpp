/*
 * HybridAnomalityDetector.cpp
 *
 * Author: Avital Livshitz 318850575
 */
#include "HybridAnomalyDetector.h"

void HybridAnomalyDetector::addCorrelationIfNeeded(const TimeSeries &ts, maxCorrelation &maxCorrelation) {
    SimpleAnomalyDetector::addCorrelationIfNeeded(ts, maxCorrelation);
    float pears = fabs(maxCorrelation.pearson);
    //we need to save only strong correlated features.
    if (pears >= 0.5 && pears <= 0.9) {
        //create array of points to find the min circle
        const auto &secFeat = ts.getFeatureNameFromPos(maxCorrelation.second);
        const auto &firstFeat = ts.getFeatureNameFromPos(maxCorrelation.first);
        auto x = ts.getDataFromFeature(firstFeat).data();
        auto y = ts.getDataFromFeature(secFeat).data();
        int size = ts.getDataFromFeature(firstFeat).size();
        Point *points[size];
        for (int i = 0; i < size; i++) {
            points[i] = new Point(x[i], y[i]);
        }
        //send to find min - circle
        Circle minCircle = findMinCircle(points, size);
        cf.push_back(correlatedFeatures{firstFeat, secFeat, pears, Line{0, 0}, minCircle.radius * 1.1f, minCircle});
        //delete the points.
        for (int i = 0; i < size; i++) {
            delete points[i];
        }
    }
}

AnomalyReport
HybridAnomalyDetector::checkAnomalityForPoint(const Point &point, const correlatedFeatures &correlatedFeatures,
                                              unsigned int timeStep) {
    //check if the point is close to the line
    if (correlatedFeatures.lin_reg.a != 0)
        return SimpleAnomalyDetector::checkAnomalityForPoint(point, correlatedFeatures, timeStep);
    //check if the point is in the min circle
    if (!isPointInCircle(point, correlatedFeatures.circle))
        return {correlatedFeatures.feature1 + "-" + correlatedFeatures.feature2, timeStep};
    return empty;
}



