#include "../Headers/Server.hpp"

void Server::handleUser(int clientFd, std::vector<std::string>& cmds)
{
    std::string messageResspons;
    if (!clients[clientFd].getPassAuthentication())
    {
        messageResspons = ":localhost 464 * :Password incorrect\r\n";
        send(clientFd, messageResspons.c_str(), messageResspons.length(), 0);
        return;
    }
    if (cmds.size() < 5)
    {
        messageResspons = ":localhost 461 * USER :Not enough parameters\r\n";
        send(clientFd, messageResspons.c_str(), messageResspons.size(), 0);
        return;
    }
    if (clients[clientFd].isAuthenticated())
    {
        messageResspons = ":localhost 462 " + clients[clientFd].getNickname() + " :Unauthorized command (already registered)\r\n";
        send(clientFd, messageResspons.c_str(), messageResspons.size(), 0);
        return;
    }
    clients[clientFd].setUsername(cmds[1]);
    clients[clientFd].setUserAuthentication(true);
    clients[clientFd].authenticate();
    if (clients[clientFd].isAuthenticated())
    {
        
        // Send the official 001 Welcome message!
        std::string nick = clients[clientFd].getNickname();
        std::string welcome = ":localhost 001 " + nick + " :Welcome to the Internet Relay Network, " + nick + "\r\n";
        send(clientFd, welcome.c_str(), welcome.length(), 0);
        
        std::cout << "User " << nick << " successfully registered!" << std::endl;
    }
}