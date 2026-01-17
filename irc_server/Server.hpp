#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <exception>

class Server
{
private:
    int _port;
    std::string _password;
    int _Server_socket_fd;
    std::vector<struct pollfd> _fds;
    
    // we use this two function to accept and handle data from client
    void acceptNewClient();
    void handleClientData(size_t& i);

public:
    Server(int _port, std::string _password);
    Server(const Server& ob);
    ~Server();
    Server& operator=(const Server& ob);

    void init();
    void run();
};