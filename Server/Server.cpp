#include "../Headers/Server.hpp"


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
    struct pollfd serverPfd;
    serverPfd.fd = serverSocket;
    serverPfd.events = POLLIN;
    serverPfd.revents = 0;
    pollfds.push_back(serverPfd);
    run();
}

void Server::run()
{
    while (true)
    {
        std::cout << "here" << "Waiting for events..." << std::endl;
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
            else if (pollfds[i].revents & (POLLHUP | POLLERR))
            {
                std::cout << "Client disconnected: " << pollfds[i].fd << std::endl;
                close(pollfds[i].fd);
                pollfds.erase(pollfds.begin() + i);
                clients.erase(pollfds[i].fd);
                --i; // Adjust index after erasing
            }
        }
    }
}

void Server::acceptConnection()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int clientFd = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
    if (clientFd < 0)
    {
        std::cerr << "Error: accept() failed to accept a new connection." << std::endl;
        return;
    }
    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    struct pollfd clientPfd;
    clientPfd.fd = clientFd;
    clientPfd.events = POLLIN;
    clientPfd.revents = 0;
    pollfds.push_back(clientPfd);
    clients[clientFd] = Client(clientFd);
    std::cout << "New client connected: " << clientFd << std::endl;
}

// Server::authenticateClient(Client &client, const std::string &password)
// {
//     if (password == this->password)
//     {
//         client.authenticate(this->password);
//     }
// }

void Server::handleClient(int clientFd)
{
    char buffer[1024];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == -1)
    {
        std::cerr << "Error: recv() failed to receive data from client " << clientFd << std::endl;
        return;
    }
    else if (bytesRead == 0)
    {
        std::cout << "Client disconnected: " << clientFd << std::endl;
        close(clientFd);
        for (std::vector<struct pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); ++it)
        {
            if (it->fd == clientFd)
            {
                pollfds.erase(it);
                break;
            }
        }
        clients.erase(clientFd);
        return;
    }
    else
    {
        buffer[bytesRead] = '\0';
        std::string bufferString(buffer, bytesRead);
        clients[clientFd].setclientBuffer(bufferString);
        
        
    }
    std::cout << "Received from client " << clientFd << ": " << buffer << std::endl;
} /// cmd\n cmd2\n cmd3\r\n
