/*
 * SimpleAnomaityDetector.cpp
 *
 * Author: Avital Livshitz 318850575
 */
#include "SimpleAnomalyDetector.h"

//Using TimeSeries, this function finds which features have strong correlation, and saves them.
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    const auto &features = ts.getFeatures();
    float pears, maxPears = 0.0;
    unsigned int first = 0, second = 1, maxSecond;
    //to each feature check the feature with the strongest correlation.
    for (auto itFeat = features.begin(); itFeat != features.end(); itFeat++) {
        for (auto itComp = itFeat + 1; itComp != features.end(); itComp++) {
            pears = pearson(ts.getDataFromFeature(*itFeat).data(),
                            ts.getDataFromFeature(*itComp).data(), ts.getDataFromFeature(*itFeat).size());
            if (fabs(maxPears) < fabs(pears)) {
                //saves the better correlated feature
                maxPears = pears;
                maxSecond = second;
            }
            second++;
        }
        //we need to save only strong correlated features.
        if (fabs(maxPears) >= 0.9) {
            cf.push_back(getCorrelationFeatures(*itFeat, features.at(maxSecond),
                                                ts.getDataFromFeature(*itFeat).data(),
                                                ts.getDataFromFeature(features.at(maxSecond)).data(),
                                                ts.getDataFromFeature(*itFeat).size(), maxPears));

            }
        second = 1 + ++first;
        maxPears = 0.0;
    }
}

//According to what features the SimpleAnormalityDetector knows are correlated,
//the function return a vector of anomaly features correlation.
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anReport;
    for_each(this->cf.begin(), this->cf.end(), [&ts, &anReport](const correlatedFeatures &corFeatures) {
        unsigned int timeStep = 1;
        const auto &vecFeature1 = ts.getDataFromFeature(corFeatures.feature1);
        const auto &vecFeature2 = ts.getDataFromFeature(corFeatures.feature2);
        auto iterator2 = vecFeature2.begin(), iterator1 = vecFeature1.begin();
        for (; iterator1 != vecFeature1.end() && iterator2 != vecFeature2.end(); iterator1++) {
            Point point = Point(*iterator1, *iterator2);
            float devP = dev(point, corFeatures.lin_reg);
            if (devP > corFeatures.threshold) {
                anReport.emplace_back(corFeatures.feature1 + "-" + corFeatures.feature2, timeStep);
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
SimpleAnomalyDetector::getCorrelationFeatures(const string &first, const string &second, const float *x, const float *y,
                                              int size,
                                              const float pearson) {
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
    return correlatedFeatures{first, second, pearson, linear, maxDis * 1.1f};
}
