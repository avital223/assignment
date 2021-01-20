/*
 * Server.c
 *
 * Author: Avital Livshitz 318850575
 */
#include <sys/socket.h>
#include "Server.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>


Server::Server(int port) throw(const char *) {
    //set port of the socket
    this->port = port;
    //create new socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
        throw "Coudn't open socket";
    //find the IP adress of localhost
    hostent *record = gethostbyname("localhost");
    auto *address = (in_addr *) record->h_addr;
    ip = inet_ntoa(*address);
    //prepare the sockaddr_in
    struct sockaddr_in serverAdd;
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = htons(port);
    if (!inet_aton(ip.c_str(), &serverAdd.sin_addr)) {
        throw "Invalid IP adress";
    }
    //assign the socket to an IP adress and a port
    if (bind(socket_fd, (const sockaddr *) &serverAdd, sizeof(serverAdd)) < 0) {
        throw "Bind failed.";
    }
}

void Server::start(ClientHandler &ch) throw(const char *) {
    //start the server in new thread - using the function threadFunc
    t = new thread(&Server::threadFunc, this, ref(ch));
}

void Server::stop() {
    t->join(); // do not delete this!
    delete t;
}

Server::~Server() {
    close(socket_fd);
}

void Server::threadFunc(ClientHandler &ch) {
    //try to connect to a client - get client address.
    struct sockaddr_in clientAdd;
    socklen_t length = sizeof(clientAdd);
    //set time for waiting
    struct timeval time{1, 0};
    //listening to a new connection.
    if (listen(socket_fd, 0) < 0)
        throw "Error when listening to new Client";
    while (true) {
        //wait for 1 second to accept data.
        if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &time, sizeof(time)) < 0)
            throw "error";
        //check if we indeed found a connection
        client_fd = accept(socket_fd, (sockaddr *) &clientAdd, &length);
        if (client_fd < 0) {
            break;
        }
        //treat the client in different thread.
        auto f = [](ClientHandler &ch, int c_fd) {
            ch.handle(c_fd);
            close(c_fd);
        };
        thread(f, ref(ch), client_fd).detach();
    }
}


