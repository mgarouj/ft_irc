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

        
    // else if (command == "PING")
    //     handlePing(clientFd, cmds);
    // else if (command == "QUIT")
    //     handleQuit(clientFd, cmds);
    else if (command == "PRIVMSG")
        handlePrivmsg(clientFd, cmds);
    // else if (command == "NOTICE")
    //     handleNotice(clientFd, cmds);
    else if (command == "JOIN")
        handleJoin(clientFd, cmds);
    // else if (command == "PART")
    //     handlePart(clientFd, cmds);
    else if (command == "MODE")
        handleMode(clientFd, cmds);
    else
    {
        std::string respons = ":localhost 421 * " + command + " :Unknown command\r\n";
        send(clientFd, respons.c_str(), respons.length(), 0);
    }
    std::cout << "pass : " << clients[clientFd].getPassAuthentication() << std::endl;
    clients[clientFd].clearBuffer();
}

void Server::handlePass(int clientFd, std::vector<std::string>& cmds)
{
    std::string respons ;
    std::cout << "==========size of cmds: " << cmds.size() << std::endl;
    if (cmds.size() < 2)
    {
        respons = "<command> :Not enough parameters";
        send(clientFd, respons.c_str(), respons.length(), 0);
        return;
    }
    if (clients[clientFd].isAuthenticated())
    {
        respons = ":You may not reregister";
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
        clients[clientFd].setPassAuthentication(false);
    }

}