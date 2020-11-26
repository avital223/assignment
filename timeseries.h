

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <fstream>
#include <cstdio>
#include <map>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class SplitString {
public:
    /**
     * Adds to the vector a list of sub-strings in myText, separated by the delimiter ch.
     *
     * @param myText the text we want to split.
     * @param ch the delimiter we want to split myText by.
     * @param splitStr the vector we put the sub-strings in.
     */
    static void splitString(const string &myText, const char &ch, vector<string>& splitStr);
};

class TimeSeries {
public:
    /**
     *Constructor of TimeSeries from CSV file.
     *
     * @param CSVfileName  a string of the name of the file.
     */
    TimeSeries(const char *CSVfileName);

     /**
      * Returns a vector of all the features per order (of appearance).
      *
      * @return a vector of the features.
      */
    const vector<string> &getFeatures() const;

    /**
     * Returns all th data associated to the feature send (if exists).
     * if the feature doesn't exist - it returns an empty vector.
     *
     * @param feature the string name of the feature.
     * @return a vector of the data associated with the feature
     */
    const vector<float> &getDataFromFeature(const string &feature)const;

    /**
     * Adds the feature with the data associated with it.
     *
     * @param feature a string name of the new feature.
     * @param data the data associated with the feature.
     */
    void addFeature(const string &feature, vector<float> &data);

    /**
     * Adds the feature with empty data.
     *
     * @param feature a string name of the new feature
     */
    void addFeature(const string &feature);


private:

    /**
     * all the features with the data associated to each one.
     */
    map<string, vector<float>> table;

    /**
     * all the features.
     */
    vector<string> features;

    /**
     * Creates a table that connects each feature to it's data according to the name of the
     * csv file.
     *
     * @param CSVfileName a string of the name of the file.
     */
    void getTableFromFile(const char *CSVfileName);

    /**
     * Sets new features according the the string myText.
     *
     * @param myText string of features.
     */
    void setFeatures(const string &myText);

    /**
     * sets new line of data and adds each data to the right features.
     *
     * @param myText the string of the data.
     */
    void setDataInTable(const string &myText);


};


#endif /* TIMESERIES_H_ */
