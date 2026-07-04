#include "../Headers/Server.hpp"

void Server::handlePass(int clientFd, std::vector<std::string>& cmds)
{
    if (cmds.size() < 2)
    {
        sendError(clientFd, 461, "PASS");
        return;
    }
    if (clients[clientFd].isAuthenticated())
    {
        sendError(clientFd, 462);
        return;
    }
    if (cmds[1] == password)
    {
        clients[clientFd].setPassAuthentication(true);
        clients[clientFd].setPassword(password);
    }
    else  
    {
        clients[clientFd].setPassAuthentication(false);
        sendError(clientFd, 464);
    }
}