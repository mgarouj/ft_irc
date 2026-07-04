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
    clients[clientFd].setHost(cmds[2]);
    clients[clientFd].setServername(cmds[3]);
    clients[clientFd].setUserAuthentication(true);
    clients[clientFd].authenticate();
    if (clients[clientFd].isAuthenticated())
    {
        std::string nick = clients[clientFd].getNickname();
        std::string welcome = ":localhost 001 " + nick + " :Welcome to the Internet Relay Network, " + nick + "\r\n";
        send(clientFd, welcome.c_str(), welcome.length(), 0);
        std::cout << "User " << nick << " successfully registered!" << std::endl;
        // sendMsg(clientFd, 001,clients[clientFd].getNickname() + "Welcome to the Internet Relay Network " + clients[clientFd].getNickname() + "! " + clients[clientFd].getNickname() + "@localhost");
        // sendMsg(clientFd, 002,clients[clientFd].getNickname() + " :Your host is ircserv, running version 1.0");
        // sendMsg(clientFd, 003,clients[clientFd].getNickname() + " :This server was created Feb 28 2026");
        // sendMsg(clientFd, 004,clients[clientFd].getNickname() + " ircserv 1.0 oi itkl");
    }
}