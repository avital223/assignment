
#include <sys/socket.h>
#include "Server.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

Server::Server(int port) throw(const char *) {
    this->port = port;
    //ip = "127.0.0.1";
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
        throw "Coudn't open socket";
    hostent *record = gethostbyname("localhost");
    auto *address = (in_addr *) record->h_addr;
    ip = inet_ntoa(*address);
//    struct sockaddr_in serverAdd;
    struct sockaddr_in serverAdd;
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = htons(port);
    if (!inet_aton(ip.c_str(), &serverAdd.sin_addr)) {
        throw "Invalid IP adress";
    }
    if (bind(socket_fd, (const sockaddr *) &serverAdd, sizeof(serverAdd)) < 0) {
        throw "Bind failed.";
    }
}

void Server::start(ClientHandler &ch) throw(const char *) {
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
    struct sockaddr_in clientAdd;
    socklen_t length = sizeof(clientAdd);
    struct timeval time{.tv_sec=1, .tv_usec=0};
    if (listen(socket_fd, 0) < 0)
        throw "Error when listening to new Client";
    while (true) {
        if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &time, sizeof(time)) < 0)
            throw "error";
        client_fd = accept(socket_fd, (sockaddr *) &clientAdd, &length);
        cout << client_fd << endl;
        if (client_fd < 0) {
            break;
        }
        write(client_fd, "lala", 4);
       ch.handle(client_fd);
       //     thread(&ClientHandler::handle,&ch,client_fd).detach();
//        auto f = [](ClientHandler &ch, int c_fd) {
//            ch.handle(c_fd);
//            close(c_fd);
//        };
////        thread(f, ref(ch), client_fd).detach();
    }
}


