/*
 * HybridAnomalityDetector.h
 *
 * Author: Avital Livshitz 318850575
 */
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
    /**
     * Constructor to HybridAnomalityDetector.
     */
    HybridAnomalyDetector() = default;

    /**
     * Destructor to HybridAnomalityDetector.
     */
    ~HybridAnomalyDetector() override = default;
private:
    /**
     * Virtual method - checks if the point is out of the normal of the correlated feature.
     * @param point the point we check
     * @param correlatedFeatures the correlated features we check on
     * @param timeStep the time step of the point
     * @return an anomaly report - or the empty report we created
     */
    AnomalyReport checkAnomalityForPoint(const Point &point, const correlatedFeatures &correlatedFeatures,
                                                 unsigned int timeStep) override;

    /**
     * Virtual method - check's if the correlation
     * @param ts the Time Series
     * @param maxCorrelation the info about the max correlation
     */
    void addCorrelationIfNeeded(const TimeSeries &ts, maxCorrelation &maxCorrelation) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
