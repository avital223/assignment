/*
 * commands.h
 *
 * Author: Avital Livshitz 318850575
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <utility>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

/**
 * The Default abstract class of IO stream
 */
class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}
};

class StandardIO : public DefaultIO {
    string read() override {
        string str;
        cin >> str;
        return str;
    }

    void write(float f) override {
        cout << fixed << setprecision(3) << f;
    }

    void write(string text) override {
        cout << text;
    }

    void read(float *f) override {
        cin >> *f;
    }
};

/**
 * an Abstract class of Command.
 */
class Command {
protected:
    //the IO to connect to the Client
    DefaultIO *dio;
    string commandDescription;// the description of the command

public:
    /**
     * Constructor of the Command Class
     * @param dio the IO Device in which we communicate
     */
    Command(DefaultIO *dio) : dio(dio) {}

    /**
     * Pure Virtual method to execute a command.
     */
    virtual void execute() = 0;

    /**
     *  print the description of the Command
     */
    void printDes() {
        dio->write(commandDescription);
    }

    /**
     * Destructor of the Command
     */
    virtual ~Command() {}
};

/**
 * all the information about the AnomalyDetector
 */
class AnomalyCommandInfo {
    //Hybrid Anomality Detector
    HybridAnomalyDetector hAD;
    //the anomality reports that where found
    vector<AnomalyReport> anomalityReports = {};
    //the number of the reports.
    unsigned int numTimeStemp = 0;
public:
    /**
     * Constructor of the AnomalyCommandInfo
     * @param hAD the HybridAnomalyDetector
     */
    AnomalyCommandInfo(const HybridAnomalyDetector &hAD) : hAD(hAD) {}

    /**
     * Destructor of the AnomalyCommandInfo
     */
    ~AnomalyCommandInfo() {
        anomalityReports.clear();
    }

    /**
     * Set the number of the reports.
     * @param timeStemp the new number of reports
     */
    void setTimeStemp(int timeStemp) { numTimeStemp = timeStemp; }

    /**
     * Get the number of the reports.
     * @return num of reports
     */
    unsigned int getTimeStemp() { return numTimeStemp; }

    /**
     * Get the HybridAnomalyDetector
     * @return the HybridAnomalyDetector
     */
    HybridAnomalyDetector &getDetactor() { return hAD; }

    /**
     * Set the new Anomality Reports;
     * @param a the new Array of reports.
     */
    void setReport(vector<AnomalyReport> a) {
        anomalityReports.clear();
        anomalityReports = std::move(a);
    }

    /**
     * Get the Anomaly reports
     * @return the AnomalyReports
     */
    vector<AnomalyReport> &getReport() { return anomalityReports; }
};

/**
 * Virtual Class for the Anomaly Detector Commands
 */
class AnomalyDetectorCommand : public Command {
protected:
    //saves all the information needed about the Anomaly Detector
    AnomalyCommandInfo *commandInfo;
public:
    /**
     * Constructor of the AnomalyDetectorCommand
     * @param dio the IO Device in which we communicate to Command constructor
     * @param hAD the Anomaly Detector Info we need.
     */
    AnomalyDetectorCommand(DefaultIO *dio, AnomalyCommandInfo *hAD) : Command(dio), commandInfo(hAD) {}
};

/**
 * The class Uploads new files to the server. Implements the AnomalyDetectorCommand
 */
class UploadCommand : public AnomalyDetectorCommand {
    //saves the description of the file and the file name.
    vector<pair<string, string>> files = {{"train CSV", "anomalityTrain.csv"},
                                          {"test CSV",  "anomalityTest.csv"}};;
public:
    //Virtual constructor for the Upload Command
    UploadCommand(DefaultIO *dio, AnomalyCommandInfo *hAD) : AnomalyDetectorCommand(dio, hAD) {
        commandDescription = "upload a time series csv file";
    }

    //virtual method of the method execute
    void execute() override {
        //upload all the files that needed.
        for_each(files.begin(), files.end(), [this](pair<string, string> &file) {
            dio->write("Please upload your local " + file.first + " file.\n");
            string line = dio->read();
            ofstream out;
            out.open(file.second);
            //read all the lines until done.
            while (line != "done") {
                out << line + "\n";
                line = dio->read();
            }
            dio->write("Upload complete.\n");
        });
    }
};

/**
 *   The class changes the settings of the Anomaly Detector. Implements the AnomalyDetectorCommand
 */
class AlgoSettingsCommand : public AnomalyDetectorCommand {
public:

    //Virtual constructor for the AlgoSettingsCommand
    AlgoSettingsCommand(DefaultIO *dio, AnomalyCommandInfo *hAD) : AnomalyDetectorCommand(dio, hAD) {
        commandDescription = "algorithm settings";
    }

    //virtual method of the method execute
    void execute() override {
        HybridAnomalyDetector hAD = commandInfo->getDetactor();
        //try read a number until we get a good number.
        float threhold = -1;
        while (threhold < 0 || threhold > 1) {
            dio->write("The current correlation threshold is ");
            dio->write(hAD.getThresholdCorrelation());
            dio->write("\nType a new threshold\n");
            dio->read(&threhold);
        }
        hAD.setMinPears(threhold);
    }
};

/**
 *   The class Learns the normal mode and detects Anomalities. Implements the AnomalyDetectorCommand
 */
class DetectAnomaliesComand : public AnomalyDetectorCommand {
public:

    //Virtual constructor for the DetectAnomaliesComand
    DetectAnomaliesComand(DefaultIO *dio, AnomalyCommandInfo *hAD) : AnomalyDetectorCommand(dio, hAD) {
        commandDescription = "detect anomalies";
    }

    //virtual method of the method execute
    void execute() override {
        HybridAnomalyDetector hAD = commandInfo->getDetactor();
        TimeSeries ts("anomalityTrain.csv");
        hAD.learnNormal(ts);
        TimeSeries test("anomalityTest.csv");
        //save the Anomaly reports
        commandInfo->setReport(hAD.detect(test));
        commandInfo->setTimeStemp(ts.getDataFromFeature(ts.getFeatureNameFromPos(0)).size());
        dio->write("anomaly detection complete.\n");
    }
};


/**
 *   The class displays the detected Anomalities. Implements the AnomalyDetectorCommand
 */
class DisplayCommand : public AnomalyDetectorCommand {
public:

    //Virtual constructor for the DisplayCommand
    DisplayCommand(DefaultIO *dio, AnomalyCommandInfo *hAD) : AnomalyDetectorCommand(dio, hAD) {
        commandDescription = "display results";
    }

    //virtual method of the method execute
    void execute() override {
        vector<AnomalyReport> anomalyReports = commandInfo->getReport();
        for_each(anomalyReports.begin(), anomalyReports.end(), [this](AnomalyReport &ar) {
            dio->write(to_string(ar.timeStep) + " \t" + ar.description + "\n");
        });
        dio->write("Done.\n");
    }
};


/**
 *   The class Uploads from the user the suspected anomalities and checks them. Implements the AnomalyDetectorCommand
 */
class UplaodAndAnalyzeCommand : public AnomalyDetectorCommand {
private:
    /**
     * checks if the middle is between point a and b.
     * @param middle the num we check
     * @param a the minimum side, (a<b)
     * @param b the moximu side
     * @return if middle is between a and b.
     */
    bool betweenTwoNums(int middle, int a, int b) {
        return (middle >= a && middle <= b);
    }

public:

    //Virtual constructor for the UplaodAndAnalyzeCommand
    UplaodAndAnalyzeCommand(DefaultIO *dio, AnomalyCommandInfo *hAD) : AnomalyDetectorCommand(dio, hAD) {
        commandDescription = "upload anomalies and analyze results";

    }

    //virtual method of the method execute
    void execute() override {
        vector<AnomalyReport> a = commandInfo->getReport();
        auto iterA = a.begin();
        auto endV = a.end();
        vector<pair<int, int>> timeStepAnomalies = {};
        pair<int, int> check = {-1, -1};
        unsigned int countN = commandInfo->getTimeStemp();
        string description = "";
        //check all the ranges of the anomalies and saves them in timeStepAnomalies
        while (iterA != endV) {
            int timeStep = iterA->timeStep;
            if (description != iterA->description || check.second + 1 != timeStep) {
                if (check.first != -1) {
                    timeStepAnomalies.push_back(check);
                }
                description = iterA->description;
                check.first = timeStep;
                check.second = check.first;
            } else {
                check.second = timeStep;
            }
            iterA++;
        }
        if (check.first != -1) {
            timeStepAnomalies.push_back(check);
        }
        // read all the lines that the user says he thinks where Anomalies
        float countFP = 0, countTP = 0, countP = 0;
        //to count all the anomalies that weren't detected at all.
        bool anomalyDetected[timeStepAnomalies.size()];
        dio->write("Please upload your local anomalies file.\r\n");
        string line = dio->read();
        while (line != "done") {
            vector<string> vector;
            countP++;
            auto itA = timeStepAnomalies.begin();
            bool countInLoop = false;
            SplitString::splitString(line, ',', vector);
            int start = stof(vector.at(0));
            int end = stof(vector.at(1));
            countN -= (end - start + 1);
            int i = 0;
            //check if the guess detected any Anomalies
            while (itA != timeStepAnomalies.end()) {
                if (betweenTwoNums(itA->first, start, end) || betweenTwoNums(itA->second, start, end) ||
                    betweenTwoNums(start, itA->first, itA->second) || betweenTwoNums(start, itA->first, itA->second)) {
                    countInLoop = true;
                    anomalyDetected[i] = true;
                }
                i++;
                itA++;
            }
            //if they where detected - count 1.
            if (countInLoop)
                countTP++;
            line = dio->read();
        }
        //count all the anomalies that weren't detected.
        for (int i = 0; i < timeStepAnomalies.size(); i++) {
            if (!anomalyDetected[i])
                countFP++;
        }
        //round to 3 numbers after point.
        float resaultFPR = floor(countFP * 1000 / (float) countN);
        float resaultTPR = floor(countTP * 1000 / countP);
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        dio->write(resaultTPR / 1000);
        dio->write("\nFalse Positive Rate: ");
        dio->write(resaultFPR / 1000);
        dio->write("\n");
    }

};

/**
 *   The class Exits the session. Implements the AnomalyDetectorCommand
 */
class ExitCommand : public AnomalyDetectorCommand {
public:

    //Virtual constructor for the UplaodAndAnalyzeCommand
    ExitCommand(DefaultIO *dio, AnomalyCommandInfo *hAD) : AnomalyDetectorCommand(dio, hAD) {
        commandDescription = "exit";
    }

    //virtual method of the method execute
    void execute() override {
        delete commandInfo;
    }
};

#endif /* COMMANDS_H_ */
