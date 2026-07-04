#include "../Headers/Server.hpp"

void Server::handleUser(int clientFd, std::vector<std::string>& cmds)
{
    if (!clients[clientFd].getPassAuthentication())
    {
        sendError(clientFd, 451);
        return;
    }
    
    if (cmds.size() < 5)
    {
        sendError(clientFd, 461, "USER");
        return;
    }
    
    if (clients[clientFd].isAuthenticated())
    {
        sendError(clientFd, 462);
        return;
    }

    clients[clientFd].setUsername(cmds[1]);

    std::string realname = cmds[4];
    for (size_t i = 5; i < cmds.size(); ++i)
    {
        realname += " " + cmds[i];
    }
    if (!realname.empty() && realname[0] == ':')
    {
        realname = realname.substr(1);
    }
    clients[clientFd].setRealname(realname);

    clients[clientFd].setUserAuthentication(true);
    clients[clientFd].authenticate();

    if (clients[clientFd].isAuthenticated())
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