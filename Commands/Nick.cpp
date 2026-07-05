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
    clients[clientFd].authenticate();
    if (clients[clientFd].isAuthenticated() && !oldNick.empty())
    {
        std::string notify = ":" + oldNick + " NICK :" + newNick + "\r\n";
        broadcast(notify);
    }
    else if (clients[clientFd].isAuthenticated() && oldNick.empty())
    {
        std::string nick = clients[clientFd].getNickname();
        std::string user = clients[clientFd].getUsername();
        std::string host = clients[clientFd].getHost();
        std::ostringstream oss;

        oss << ":" << this->serverName << " 001 " << nick 
            << " :Welcome to the Internet Relay Network " << nick << "!" << user << "@" << host << "\r\n";
            
        oss << ":" << this->serverName << " 002 " << nick 
            << " :Your host is " << this->serverName << ", running version 1.0\r\n";
            
        oss << ":" << this->serverName << " 003 " << nick 
            << " :This server was created today\r\n";
            
        oss << ":" << this->serverName << " 004 " << nick 
            << " " << this->serverName << " 1.0 o o\r\n";

        std::string welcomeMsgs = oss.str();
        send(clientFd, welcomeMsgs.c_str(), welcomeMsgs.length(), 0);
        
        std::cout << "User " << nick << " successfully registered!" << std::endl;
    }
}