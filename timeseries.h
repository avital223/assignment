

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <fstream>
#include <stdio.h>
#include <map>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class SplitString {
public:
    static vector<string> splitString(const string &myText, const char &ch);
};

class TimeSeries {
public:
    TimeSeries(const char *CSVfileName);

    void addKeyValue(const string keyValue);

    void addKeyValue(const string keyValue, const vector<float> values);

    const vector<float> &getValuesFromKey(const string &keyValue) const;

    const vector<std::string> &getKeyValues() const;

private:

    map<string, vector<float>> table;

    vector<string> keyValues;

    vector<string> setKeyValues(const string &myText);

    void setValuesInTable(const string &basicString);

    void getTableFromFile(const char *CSVfileName);
};


#endif /* TIMESERIES_H_ */
