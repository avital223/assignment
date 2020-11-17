#include <iostream>
#include "timeseries.h"

const vector<string> &SplitString::splitString(const string &myText, const char &ch) {
    vector<string> splitStr;
    size_t current, previous = 0;
    current = myText.find(ch);
    while (current != std::string::npos) {
        splitStr.push_back(move(myText.substr(previous, current - previous)));
        previous = current + 1;
        current = myText.find(',', previous);
    }
    splitStr.push_back(move(myText.substr(previous, current - previous)));
    return splitStr;
}

TimeSeries::TimeSeries(const char *CSVfileName) {
    getTableFromFile(CSVfileName);
}

void TimeSeries::getTableFromFile(const char *CSVfileName) {
    string myText, strTable;
    ifstream MyReadFile(CSVfileName);
    if (!MyReadFile.is_open()) {
        throw "Couldn't open the file :" + *CSVfileName;
    }
    if (getline(MyReadFile, myText)) {
        setKeyValues(myText);
    }
    while (getline(MyReadFile, myText)) {
        setValuesInTable(myText);
    }
    MyReadFile.close();
}

vector<string> TimeSeries::setKeyValues(const string &myText) {
    this->keyValues = move(SplitString::splitString(myText, ','));
    vector<string>::iterator it;
    for (it = keyValues.begin(); it != keyValues.end(); it++) {
        table.insert(pair<string, vector<float>>(*it, vector<float>()));
    }
    return keyValues;
}

void TimeSeries::setValuesInTable(const string &myText) {
    vector<string> values = SplitString::splitString(myText, ',');
    vector<string>::iterator itKeys;
    vector<string>::iterator itVal = values.begin();
    for (itKeys = keyValues.begin(); itKeys != keyValues.end() && itVal != values.end(); itKeys++) {
        table[*itKeys].push_back(stof(*itVal));
        itVal++;
    }
}

const vector<string> &TimeSeries::getKeyValues() const { return keyValues; }

const vector<float> &TimeSeries::getValuesFromKey(const string &keyValue) const {
    std::vector<string>::const_iterator iter;
    for (iter = this->keyValues.begin(); iter != this->keyValues.end(); iter++) {
        if (!keyValue.compare(*iter))
            return this->table.at(keyValue);
    }
    return {};
}

void TimeSeries::addKeyValue(const string keyValue) {
    vector<float> vec;
    vec.resize(table[*keyValues.begin()].size(), 0);
    table.insert(pair<string, vector<float>>(keyValue, vec));
}

void TimeSeries::addKeyValue(const string keyValue, const vector<float> values) {
    table.insert(pair<string, vector<float>>(keyValue, values));
}