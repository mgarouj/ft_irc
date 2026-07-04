#include "../Headers/Server.hpp"


void Server::broadcast(const std::string& message)
{
    std::map<int, Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it) 
    {
        int targetFd = it->first;
        send(targetFd, message.c_str(), message.length(), 0);
    }
}

void Server::handleNick(int clientFd, std::vector<std::string>& cmds)
{
    if (!clients[clientFd].getPassAuthentication())
    {
        sendError(clientFd, 451);
        return;
    }
    if (cmds.size() < 2 || cmds[1].empty())
    {
        sendError(clientFd, 431);
        return;
    }
    std::string newNick = cmds[1];

    if (newNick.length() > 9) 
        newNick = newNick.substr(0, 9);
    std::string specialChars = "-[]\\`^{}_"; 
    if (!std::isalpha(newNick[0]) && specialChars.find(newNick[0]) == std::string::npos)
    {
        sendError(clientFd, 432, newNick);
        return;
    }
    for (size_t i = 1; i < newNick.length(); ++i)
    {
        if (!std::isalnum(newNick[i]) && specialChars.find(newNick[i]) == std::string::npos)
        {
            sendError(clientFd, 432, newNick);
            return;
        }
    }
    for (std::map<int, Client>::iterator i = clients.begin(); i != clients.end(); i++)
    {
        if (i->second.getNickname() == newNick && i->first != clientFd)
        {
            sendError(clientFd, 433, newNick);
            return;
        }
    }
    std::string oldNick = clients[clientFd].getNickname();
    clients[clientFd].setNickname(newNick);
    clients[clientFd].setNickAuthentication(true);
    if (clients[clientFd].isAuthenticated() && !oldNick.empty())
    {
        std::string notify = ":" + oldNick + " NICK :" + newNick + "\r\n";
        broadcast(notify);
    }
}