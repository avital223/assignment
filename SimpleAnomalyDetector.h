/*
 * SimpleAnomaityDetector.h
 *
 * Author: Avital Livshitz 318850575
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
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
};


class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
    /**
     * All the important correlations between the features.
     */
    vector<correlatedFeatures> cf;
public:
    /**
     * Constructor of SimpleAnormalityDetector.
     */
    SimpleAnomalyDetector() = default;

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
    getCorrelationFeatures(const string &first, const string &second, const float *x, const float *y, int size,
                           const float pearson);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
