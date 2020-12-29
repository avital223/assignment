/*
 * timeseries.cpp
 *
 * Author: Avital Livshitz 318850575
 */
#include <iostream>
#include "timeseries.h"

//Adds to the vector a list of sub-strings in myText, separated by the delimiter ch.
void SplitString::splitString(const string &myText, const char &ch, vector<string> &splitStr) {
    unsigned long current, previous = 0;
    current = myText.find(ch);
    //go until you no longer find the char ch in the string myText.
    while (current != -1) {
        splitStr.push_back(move(myText.substr(previous, current - previous)));
        previous = current + 1;
        //search from the last place we saw the char.
        current = myText.find(ch, previous);
    }
    splitStr.push_back(move(myText.substr(previous, current - previous)));
}

//Constructor of TimeSeries from CSV file.
TimeSeries::TimeSeries(const char *CSVfileName) {
    getTableFromFile(CSVfileName);
}

//Creates a table that connects each feature to it's data according to the name of the csv file.
void TimeSeries::getTableFromFile(const char *CSVfileName) {
    string myText, strTable;
    ifstream myReadFile(CSVfileName);
    //if we weren't able to open the file
    if (!myReadFile.is_open()) {
        throw "Couldn't open the file :" + *CSVfileName;
    }
    //the first line contains our features
    if (getline(myReadFile, myText)) {
        setFeatures(myText);
    }
    //each new line contains data to each of the features.
    while (getline(myReadFile, myText)) {
        setDataInTable(myText);
    }
    myReadFile.close();
}

//Sets new features according the the string myText.
void TimeSeries::setFeatures(const string &myText) {
    SplitString::splitString(myText, ',', this->features);
    for_each(features.begin(), features.end(), [this](const string &feature) {
        addFeature(feature);
    });
}

//sets new line of data and adds each data to the right features.
void TimeSeries::setDataInTable(const string &myText) {
    vector<string> values;
    SplitString::splitString(myText, ',', values);
    vector<string>::iterator itKeys;
    vector<string>::iterator itVal = values.begin();
    //we put the data in the order of it's appearance.
    for (itKeys = features.begin(); itKeys != features.end(); itKeys++) {
        if (itVal != values.end()) {
            table[*itKeys].push_back(stof(*itVal));
            itVal++;
        } else
            //in case of missing data - it's better to add 0 so there wouldn't be mismatch of the table.
            table[*itKeys].push_back(0);

    }
}

//Returns a vector of all the features per order (of appearance).
const vector<string> &TimeSeries::getFeatures() const { return features; }

//Returns all th data associated to the feature send (if exists).
//if the feature doesn't exist - it returns an empty vector.
const vector<float> &TimeSeries::getDataFromFeature(const string &feature) const {
    std::vector<string>::const_iterator iter;
    iter = find(features.begin(), features.end(), feature);
    if (iter == features.end())
        return vector<float>();
    return this->table.at(feature);
}

//Adds the feature with empty data.
void TimeSeries::addFeature(const string &feature) {
    vector<float> vec;
    //if the table is empty - it'll insert an empty vector
    vec.resize(table[*features.begin()].size(), 0);
    table.emplace(feature, vec);
}

//Adds the feature with the data associated with it.
void TimeSeries::addFeature(const string &feature, vector<float> &data) {
    table.emplace(feature, move(data));
}

const string &TimeSeries::getFeatureNameFromPos(int pos) const{
    return features.at(pos);
}
