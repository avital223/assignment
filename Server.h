/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
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
    virtual void handle(int clientID) = 0;
};


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {
        SocketIO dio = SocketIO(clientID);
        //StandardIO dio = StandardIO();
        CLI cli = CLI(&dio);
        cli.start();
        close(clientID);
    }
};


// implement on Server.cpp
class Server {
    thread *t{}; // the thread to run the start() method in
private:
    string ip;
    int port, socket_fd, client_fd;

    void threadFunc(ClientHandler &ch);

public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif /* SERVER_H_ */
