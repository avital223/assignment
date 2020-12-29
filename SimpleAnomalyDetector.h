/*
 * SimpleAnomaityDetector.h
 *
 * Author: Avital Livshitz 318850575
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

/**
 * All the information between two correlated features
 */
struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Circle circle;
};

struct maxCorrelation {
    int first;
    int second;
    float pearson;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
protected:
    /**
     * All the important correlations between the features.
     */
    vector<correlatedFeatures> cf;

    /**
     * the minimum correlation needed to create a correlation line.
     */
    float minPears = 0.9;

    /**
     * checks if the point is out of the normal of the correlated feature.
     * @param point the point we check
     * @param correlatedFeatures the correlated features we check on
     * @param timeStep the time step of the point
     * @return an anomaly report - or the empty report we created
     */
    virtual
    AnomalyReport
    checkAnomalityForPoint(const Point &point, const correlatedFeatures &correlatedFeatures,
                           unsigned int timeStep);

    /**
     * check's if the correlation
     * @param ts
     * @param itFeat
     */
    virtual void addCorrelationIfNeeded(const TimeSeries &ts, maxCorrelation &maxCorrelation);

/**
 * an empty Anomality report for good points.
 */
    AnomalyReport empty = {"nan", -1};
public:

    /**
     * Constructor of SimpleAnormalityDetector.
     */
    SimpleAnomalyDetector() = default;

    void setMinPears(float min);
    /**
     * Destructor of SimpleAnormalityDetector.
     */
    ~SimpleAnomalyDetector() override = default;

    /**
     * Using TimeSeries, this function finds which features have strong correlation,
     * and saves them
     *
     * @param ts the TimeSeries
     */
    virtual void learnNormal(const TimeSeries &ts);


    /**
     * According to what features the SimpleAnormalityDetector knows are correlated,
     * the function return a vector of anomaly features correlation.
     *
     * @param ts the TimeSeries we want to check
     * @return a vector of anomaly reports (of the features involved)
     */
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    /**
     * Returns the features that have a strong correlation and the information about it.
     * @return vector of the correlated features.
     */
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }


private :

    /**
     * This Function calculates all the information from the 2 features and returns
     * a correlatedFeatures that saves all this information.
     *
     * @param first the name of the first feature
     * @param second the name of the second feature
     * @param x the data of the first feature
     * @param y the data of the second feature
     * @param size the size of the both data of the 2 features.
     * @param pearson the pearson of the both data of the 2 features.
     * @return the correlated feature information
     */
    static correlatedFeatures
    getCorrelationFeatures(maxCorrelation &maxCorr, const TimeSeries &ts);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
