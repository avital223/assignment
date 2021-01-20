/*
 * Server.h
 *
 * Author: Avital Livshitz 318850575
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include <netinet/in.h>
#include <string>
#include "commands.h"
#include "CLI.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler {
public:
    //handle each cilent
    virtual void handle(int clientID) = 0;
};


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
    //handle the client of the detector
    virtual void handle(int clientID) {
        SocketIO dio = SocketIO(clientID);
        CLI cli = CLI(&dio);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
    thread *t{}; // the thread to run the start() method in
private:
    //all the needed info for the socket
    string ip;
    int port, socket_fd, client_fd;

    /**
     * the function we cann to open the thread of the server
     */
    void threadFunc(ClientHandler &ch);

public:
    /**
     * Constructor of a new server
     * @param port the port of the socket
     */
    Server(int port) throw(const char *);

    /**
     * destructor of the server
     */
    virtual ~Server();

    /**
     * start the server
     * @param ch the client handler
     */
    void start(ClientHandler &ch) throw(const char *);

    /**
     * stop the server.
     */
    void stop();
};

#endif /* SERVER_H_ */
