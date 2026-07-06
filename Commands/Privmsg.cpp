#include "../Headers/Server.hpp"

void Server::handlePrivmsg(int clientFd, std::vector<std::string>& cmds)
{
    Client* client = &clients[clientFd];
    std::string Message = "";
    if(cmds.size() == 1)
    {
        sendError(clientFd, 411, client->getNickname());
        return;
    }

    if(!getisColenExits() || cmds.size() < 3)
    {
        sendError(clientFd, 412, client->getNickname());
        return;
    }

    std::vector<std::string> tempTargets;
    std::string token;

    std::stringstream ssChannels(cmds[1]);
    while (std::getline(ssChannels, token, ',')) {
        tempTargets.push_back(token);
    }
    
    for(size_t i = 0; i < tempTargets.size(); i++)
    {
        std::string Target = tempTargets[i];
        if(Target[0] == '#')
        {
            std::map<std::string, Channel>::iterator channel = channels.find(Target);
            if(channel != channels.end())
            {
                if(!channel->second.isMember(client))
                {
                    sendError(clientFd, 404, client->getNickname());
                    continue;
                }
                else
                {
                    Message = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + " PRIVMSG " + channel->second.getName() + ":" + cmds[2] + "\r\n";
                    channel->second.broadcastMessage(Message, client);
                    continue;
                }
            }
            else
            {
                Message = client->getNickname() + " " + Target;
                sendError(clientFd, 401, Message);
                continue;
            }
        }
        else
        {
            bool userFound = false;
            for(std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
            {
                if(it->second.getNickname() == Target && it->second.isAuthenticated())
                {
                    userFound = true;
                    Message =  ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + " PRIVMSG " + it->second.getNickname() + ":" + cmds[2] + "\r\n";
                    send(it->first, Message.c_str(), Message.length(), 0);
                    break;
                }   
            }
            if(!userFound)
            {
                Message = client->getNickname() + " " + Target;
                sendError(clientFd, 401, Message);
            }
        }
    }
}