# pragma once

#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()


class Server
{
    private:
        int _port;
        std::string _password;
        int _socketFd;
    public:
        Server();
        Server(int port, std::string password);
        Server(const Server& ob);
        ~Server();
        
        Server& operator=(const Server& ob);
};