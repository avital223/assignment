
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
    HybridAnomalyDetector() = default;

    virtual ~HybridAnomalyDetector() = default;

    AnomalyReport checkAnomalityForPoint(const Point &point, const correlatedFeatures &correlatedFeatures,
                                                 unsigned int timeStep) override;

    void addCorrelationIfNeeded(const TimeSeries &ts, maxCorrelation &maxCorrelation) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
