#pragma once


#include <iostream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <sstream>
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
       std::vector<std::string> extractAndSplit(std::string &buffer);

        void executeCommand(std::vector<std::string>& cmds, int clientFd);

        void broadcast(const std::string& message);
        void handlePass(int clientFd, std::vector<std::string>& cmds);
        void handleNick(int clientFd, std::vector<std::string>& cmds);
        void handleUser(int clientFd, std::vector<std::string>& cmds);
        
    };
