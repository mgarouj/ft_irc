#include "../Headers/Server.hpp"

void Server::executeCommand(std::vector<std::string>& cmds, int clientFd)
{
    if (cmds.empty() || cmds[0].empty())
        return;

    std::string command = cmds[0].substr(0, cmds[0].size());
    for (size_t i = 0; i < command.length(); ++i)
    {
        command[i] = std::toupper(command[i]);
    }
    //check if authenticated user
    if(command != "NICK" && command != "USER" && command != "PASS")
    {
        if(!clients[clientFd].isAuthenticated())
        {
            std::string response = ":localhost 451 * :You have not registered\r\n";
            send(clientFd, response.c_str(), response.length(), 0);
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
    {
        std::string respons = "421 * " + command + " :Unknown command\r\n";
        send(clientFd, respons.c_str(), respons.length(), 0);
    }
    clients[clientFd].clearBuffer();
}

void Server::handlePass(int clientFd, std::vector<std::string>& cmds)
{
    std::string respons ;
    if (cmds.size() < 2)
    {
        respons = "461 PASS :Not enough parameters\r\n";
        send(clientFd, respons.c_str(), respons.length(), 0);
        return;
    }
    if (clients[clientFd].getPassAuthentication() == 1)
    {
        std::cout << "clients[clientFd].getPassAuthentication()" << clients[clientFd].getPassAuthentication() << std::endl;
        respons = "462 :You may not reregister\r\n";
        send(clientFd, respons.c_str(), respons.length(), 0);
        return;
    }
    std::cout << "[" << password<< "]"<<"["<< cmds[1] << "]" << std::endl;
    if (cmds[1] == password)
    {
        clients[clientFd].setPassAuthentication(true);
        clients[clientFd].setPassword(password);
    }
    else  
    {
        respons = "464 :Password incorrect\r\n";
        send(clientFd, respons.c_str(), respons.length(), 0);
        clients[clientFd].setPassAuthentication(false);
    }
}