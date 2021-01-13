/*
 * CLI.h
 *
 * Author: Avital Livshitz 318850575
 */

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    // the IO device in which we communicate to the client
    DefaultIO *dio;
private:
    // all the commands that the server can do
    vector<Command *> comands;
public:
    /**
     * Constructor to the CLI
     * @param dio the IO Device in which we communicate
     */
    CLI(DefaultIO *dio);

    /**
     * Start the session with the Client
     */
    void start();

    /**
     * Destructor of the CLI
     */
    virtual ~CLI();
};

#endif /* CLI_H_ */
