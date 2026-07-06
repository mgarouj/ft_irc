#include "../Headers/Server.hpp"
#include <sstream>

void Server::handleMode(int clientFd, std::vector<std::string>& cmds)
{
    Client* client = &clients[clientFd];
    std::string Message = "";

    if (cmds.size() < 2)
    {
        sendError(clientFd, 411, client->getNickname());
        return;
    }

    std::string Target = cmds[1];
    if (channels.find(Target) == channels.end())
    {
        Message = Target;
        sendError(clientFd, 401, Message);
        return;
    }

    Channel& channel = channels.find(Target)->second;

    if (cmds.size() == 2)
    {
        Message = ":ircserv.com 324 " + client->getNickname() + " " + Target +" (+ | -)<i,t,k,l,o>\r\n";
        send(clientFd, Message.c_str(), Message.length(), 0);
        return;
    }

    if (!channel.isOperator(client))
    {
        Message = Target;
        sendError(clientFd, 482, Message);
        return;
    }

    std::string modeString = cmds[2];
    bool isAdding = true;
    size_t argIdx = 3;

    std::string successfulModes = "";
    std::string successfulArgs = "";


    for (size_t i = 0; i < modeString.length(); i++)
    {
        char c = modeString[i];

        if (c == '+')
        {
            isAdding = true;
            successfulModes += "+";
        }
        else if (c == '-')
        {
            isAdding = false;
            successfulModes += "-";
        }
        else if (c == 'i')
        {
            channel.setinviteOnlyattr(isAdding);
            successfulModes += "i";
        }
        else if (c == 't')
        {
            channel.setTopicRestricted(isAdding);
            successfulModes += "t";
        }

        else if (c == 'k')
        {
            if (isAdding)
            {
                if (argIdx < cmds.size())
                {
                    channel.setPassword(cmds[argIdx]);
                    successfulModes += "k";
                    successfulArgs += " " + cmds[argIdx];
                    argIdx++;
                }
            }
            else
            {
                channel.setPassword("");
                successfulModes += "k";
            }
        }
        else if (c == 'o')
        {
            if (argIdx < cmds.size())
            {
                std::string targetNick = cmds[argIdx];
                bool TargetNickFound = false;
                for(std::map<int, Client>::iterator clientit = clients.begin(); clientit != clients.end(); clientit++)
                {
                    if(clientit->second.getNickname() == targetNick)
                    {
                        Client* NextOperator = &clients[clientit->second.getFd()];
                        if(isAdding)
                            channel.addOperator(NextOperator);
                        else
                            channel.removeOperator(NextOperator);
                        TargetNickFound = true;
                    }
                }
                if(!TargetNickFound)
                    continue;
                successfulModes += "o";
                successfulArgs += " " + targetNick;
                argIdx++;
            }
        }
        else if (c == 'l')
        {
            if (isAdding)
            {
                if (argIdx < cmds.size())
                {
                    size_t limit = std::atoi(cmds[argIdx].c_str());
                    if (limit > 0)
                    {
                        channel.setUserLimit(limit);
                        successfulModes += "l";
                        successfulArgs += " " + cmds[argIdx];
                    }
                    argIdx++;
                }
            }
            else
            {
                channel.setUserLimit(0);
                successfulModes += "l";
            }
        }
        else
        {
            Message = client->getNickname() + " " + c;
            sendError(clientFd, 472, Message);
        }
    }

    if (successfulModes.length() > 0 && successfulModes != "+" && successfulModes != "-")
    {
        Message = ":" + client->getNickname() + "!" + client->getUsername() + "@"+ client->getHost() + " MODE " + Target + " " + successfulModes + successfulArgs + "\r\n";
        channel.broadcastMessage(Message, NULL);
    }
}