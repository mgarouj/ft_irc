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
    std::string respons;
    if (!clients[clientFd].getPassAuthentication())
    {
        respons = ":localhost 464 * :Password incorrect\r\n";
        send(clientFd, respons.c_str(), respons.length(), 0);
        return;
    }
    if (cmds.size() < 2 || cmds[1].empty())
    {
        respons = ":localhost 431 * :No nickname given\r\n";
        send(clientFd, respons.c_str(), respons.length(), 0);
        return;
    }
    std::string newNick = cmds[1];
    if (newNick.find_first_of(" ,*?!@") != std::string::npos || newNick[0] == '#' || newNick[0] == ':') 
    {
        respons = ":localhost 432 * " + newNick + " :Erroneous nickname\r\n";
        send(clientFd, respons.c_str(), respons.length(), 0);
        return;
    }
    for (std::map<int, Client>::iterator i = clients.begin(); i != clients.end(); i++)
        if (newNick == i->second.getNickname()){
            respons = ":localhost 433 * " + newNick + " :Nickname is already in use\r\n";
            send(clientFd, respons.c_str(), respons.length(), 0);
            return;
        }

    std::string oldNick = clients[clientFd].getNickname();

    clients[clientFd].setNickname(newNick);
    clients[clientFd].setNickAuthentication(true);
    if (clients[clientFd].isAuthenticated() && !oldNick.empty())
    {
        std::string notify = ":" + oldNick + " NICK " + newNick + "\r\n";
        broadcast(notify);
    }
}