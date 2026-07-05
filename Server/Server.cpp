#include "../Headers/Server.hpp"

bool Server::isSignal = false;

void Server::CloseFds()
{
    for (size_t i = 0; i < pollfds.size(); i++)
    {
        std::cout << "Closing file descriptor: " << pollfds[i].fd << std::endl;
        close(pollfds[i].fd);
    }
}

bool Server::getSignal(){return(isSignal);}


void Server::setSignal(bool S){isSignal = S;}

void Server::signalR(int S)
{
    isSignal = true;
    if (S == SIGINT)
        std::cout << "Interrupted by SIGINT" << std::endl;
    else if (S == SIGQUIT)
        std::cout << "Interrupted by SIGQUIT" << std::endl;
    else
        std::cout << "Interrupted by signal" << std::endl;
}

Server::Server() : password(""), port(0), serverSocket(-1) {}

Server::Server(const std::string &password, const int port) : password(password), port(port), serverSocket(-1), serverName("ircserv.com"), isColenExists(0) {}

Server::Server(const Server &other) : password(other.password), port(other.port), serverSocket(other.serverSocket), serverName(other.serverName), isColenExists(other.isColenExists) {}

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

bool Server::getisColenExits() const {return(isColenExists);}

void Server::start()
{
    std::cout << "Server started on port " << port << " with password " << password << std::endl;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        throw std::runtime_error("Failed to create socket");
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw std::runtime_error("Error: setsockopt() failed");
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
    while (Server::isSignal == false)
    {
        if (poll(&pollfds[0], pollfds.size(), -1) < 0)
        {
            if (Server::isSignal == true)
                break;
            throw std::runtime_error("Error: poll() failed to monitor the file descriptors.");
        }
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
                --i;
            }
        }
    }
    CloseFds();
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
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error: fcntl() failed to set the socket to non-blocking mode." << std::endl;
		return ;
	}
    struct pollfd clientPfd;
    clientPfd.fd = clientFd;
    clientPfd.events = POLLIN;
    clientPfd.revents = 0;
    pollfds.push_back(clientPfd);
    clients[clientFd] = Client(clientFd);
    clients[clientFd].setHost(inet_ntoa(clientAddress.sin_addr));
    std::cout << "New client connected: [" << clientFd << "]" <<std::endl;
}


void Server::handleClient(int clientFd)
{
    char buffer[512];
    memset(buffer, 0, sizeof(buffer)); 

    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) 
    {
        if (bytesRead == 0)
            std::cout << "Client " << clientFd << " disconnected." << std::endl;
        else
            std::cerr << "Error: recv() failed on client " << clientFd << std::endl;
        std::string Message = ":" + clients[clientFd].getNickname() + "!" + clients[clientFd].getUsername() + "@" + clients[clientFd].getHost() + " QUIT\r\n";
        for (std::map<std::string, Channel>::iterator ite = channels.begin(); ite != channels.end(); ite++)
        {
            Client *client = &clients[clientFd];
            if (ite->second.isMember(client))
            {
                ite->second.broadcastMessage(Message, client);
                ite->second.removeMember(&clients[clientFd]);
                if(clients[clientFd].getchannels_counter() >= 0)
                    client->setchannels_counter(client->getchannels_counter() - 1);
                if(ite->second.isEmpty())
                {
                    std::map<std::string, Channel>::iterator it = channels.find(ite->first);
                    if (it != channels.end()){
                        channels.erase(it);
                    }
                    if(channels.size() == 0)
                        break;
                }
            }
        }
        
        close(clientFd);
        clients.erase(clientFd);
        for (std::vector<struct pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); ++it)
        {
            if (it->fd == clientFd)
            {
                pollfds.erase(it);
                break;
            }
        }
        return;
    }

    std::string &currentBuffer = clients[clientFd].getclientBuffer();
    currentBuffer.append(buffer, bytesRead);

    for (std::string::size_type pos = 0; (pos = currentBuffer.find("\r\n", pos)) != std::string::npos;) {
        currentBuffer.replace(pos, 2, "\n");
    }

    std::vector<std::string> commandsToRun;
    size_t pos;
    while ((pos = currentBuffer.find('\n')) != std::string::npos)
    {
       std::string singleCmd = currentBuffer.substr(0, pos);
        currentBuffer.erase(0, pos + 1);
        
        if (!singleCmd.empty())
            commandsToRun.push_back(singleCmd);
     }
    for (size_t i = 0; i < commandsToRun.size(); i++)
    {
        std::vector<std::string> cmds = extractAndSplit(commandsToRun[i]);
        executeCommand(cmds, clientFd);
        isColenExists = 0;
    }
}

std::vector<std::string> Server::extractAndSplit(std::string &buffer)
{
    std::vector<std::string> args;
    std::stringstream ss(buffer);

    std::string token;

    while (ss >> token)
    {
        if (token[0] == ':')
        {
            isColenExists = 1;
            std::string rest;
            std::getline(ss, rest);
            token.erase(0, 1);
            token += rest;
            args.push_back(token);
            break;
        }
        args.push_back(token);
    }
    return args;
}


void Server::executeCommand(std::vector<std::string>& cmds, int clientFd)
{
    if (cmds.empty() || cmds[0].empty())
        return;

    std::string command = cmds[0].substr(0, cmds[0].size());
    for (size_t i = 0; i < command.length(); ++i)
    {
        command[i] = std::toupper(command[i]);
    }

    if(command != "NICK" && command != "USER" && command != "PASS")
    {
        if(!clients[clientFd].isAuthenticated())
        {
            sendError(clientFd, 451);
            return;
        }
    }
    
    if (command == "PASS")
        handlePass(clientFd, cmds);
    else if (command == "NICK")
        handleNick(clientFd, cmds);
    else if (command == "USER")
        handleUser(clientFd, cmds);
    else if (command == "KICK")
        handleKick(clientFd, cmds);
    else if (command == "INVITE")
        handleInvite(clientFd, cmds);
    else if (command == "TOPIC")
        handleTopic(clientFd, cmds);
    else if (command == "PRIVMSG")
        handlePrivmsg(clientFd, cmds);
    else if (command == "JOIN")
        handleJoin(clientFd, cmds);
    else if (command == "MODE")
        handleMode(clientFd, cmds);
    else
        sendError(clientFd, 421, command);
    clients[clientFd].clearBuffer();
}