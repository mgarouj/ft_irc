#include "Server.hpp"


Server::Server() : password(""), port(0), serverSocket(-1) {}

Server::Server(const std::string &password, const int port) : password(password), port(port), serverSocket(-1) {}

Server::Server(const Server &other) : password(other.password), port(other.port), serverSocket(other.serverSocket) {}

Server &Server::operator=(const Server &other)
{
    if (this != &other)
    {
        password = other.password;
        port = other.port;
        serverSocket = other.serverSocket;
    }
    return *this;
}

Server::~Server() {}

void Server::start()
{
    std::cout << "Server started on port " << port << " with password " << password << std::endl;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr* )&address, sizeof(address)))
        throw std::runtime_error("Failed to bind socket");
    if (listen(serverSocket, SOMAXCONN) == -1)
        throw std::runtime_error("Error: listen() failed to open the socket.");
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: fcntl() failed to set the socket to non-blocking mode.");  
    pollfds.push_back({serverSocket, POLLIN, 0});
    run();
}

void Server::run()
{
    while (true)
    {
        if (poll(pollfds.data(), pollfds.size(), -1) < 0)
            throw std::runtime_error("Error: poll() failed to monitor the file descriptors.");
        for (size_t i = 0; i < pollfds.size(); ++i)
        {
            if (pollfds[i].revents & POLLIN)
            {
                if (pollfds[i].fd == serverSocket)
                    acceptConnection();
                else
                    handleClient(pollfds[i].fd);
            }
        }
    }
}

void Server::acceptConnection()
{
    
}