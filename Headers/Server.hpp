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
#include <map>
#include "Client.hpp"

class Server
{
    private:
        std::string password;
        int port;
        int serverSocket;
        struct sockaddr_in address;
        std::vector<struct pollfd> pollfds;
        std::map<int, Client> clients;




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
        void authenticateClient(Client &client, const std::string &password);
        std::pair<std::string, std::string> extractAndSplit(std::string &buffer);
        // void executeCommand(const std::pair<std::string, std::string> &cmdPair, Client &client);
    };
