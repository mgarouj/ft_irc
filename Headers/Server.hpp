#pragma once


#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <fcntl.h>
#include "Client.hpp"

class Server
{
    private:
        std::string password;
        int port;
        int serverSocket;
        struct sockaddr_in address;
        struct pollfd pfd;
        std::vector<struct pollfd> pollfds;
        Client *clients[1024]; // 1024 because it's the maximum number of clients that can be handled by the server


    public:
        Server();
        Server(const std::string &password, const int port);
        Server(const Server &other);
        Server &operator=(const Server &other);
        ~Server();

        void start();
        void run();
        void acceptConnection();
        void handleClient(int clientFd);


};