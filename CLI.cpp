/*
 * CLI.c
 *
 * Author: Avital Livshitz 318850575
 */
#include "CLI.h"

//Constructor to the CLI
CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    // Initilaze all the commands in the CLI
    AnomalyCommandInfo* commandInfo = new AnomalyCommandInfo(HybridAnomalyDetector());
    comands.push_back(new UploadCommand(dio, commandInfo));
    comands.push_back(new AlgoSettingsCommand(dio, commandInfo));
    comands.push_back(new DetectAnomaliesComand(dio, commandInfo));
    comands.push_back(new DisplayCommand(dio, commandInfo));
    comands.push_back(new UplaodAndAnalyzeCommand(dio, commandInfo));
    comands.push_back(new ExitCommand(dio, commandInfo));
}

//Start the session with the Client
void CLI::start() {
    string line;
    // do session untill the exit command
    while (line.empty() || line != "6") {
        int index = 1;
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        //print all the commands in the menu
        for_each(comands.begin(), comands.end(), [&index, this](Command *command) {
            dio->write(to_string(index) + ".");
            command->printDes();
            dio->write("\n");
            index++;
        });
        line = dio->read();
        //if the number was incorrect.
        try {
            int command = stoi(line);
            comands.at(command - 1)->execute();
        } catch (exception) {
        }
    }
}

//Destructor of the CLI
CLI::~CLI() {
    //delete all the commands
    for_each(comands.begin(), comands.end(), [](Command *command) {
        delete command;
    });
}

