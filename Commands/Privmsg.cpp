#include "../Headers/Server.hpp"

void Server::handlePrivmsg(int clientFd, std::vector<std::string>& cmds)
{
    Client* client = &clients[clientFd];
    std::string Message = "";
    if(cmds.size() == 1)
    {
        Message = ":localhost 411 " + client->getNickname() + " :Not enough parameters\r\n";
        send(clientFd, Message.c_str(), Message.length(), 0);
        return;
    }

    if(!getisColenExits() || cmds.size() < 3)
    {
        Message = ":localhost 412 " + client->getNickname() + " :Not enough parameters\r\n";
        send(clientFd, Message.c_str(), Message.length(), 0);
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
                    Message = ":localhost 404 " + client->getNickname() + " " + channel->second.getName() + " :Cannot send to channel\r\n";
                    send(clientFd, Message.c_str(), Message.length(), 0);
                    continue;
                }
                else
                {
                    Message = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost PRIVMSG " + channel->second.getName() + ":" + cmds[2] + "\r\n";
                    channel->second.broadcastMessage(Message, client);
                    continue;
                }
            }
            else
            {
                Message = ":localhost 401 " + client->getNickname() + " " + Target +" :No such nick/channel\r\n";
                send(clientFd, Message.c_str(), Message.length(), 0);
                continue;
            }
        }
        else
        {
            bool userFound = false;
            for(std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
            {
                if(it->second.getNickname() == Target)
                {
                    userFound = true;
                    Message =  ":" + client->getNickname() + "!" + client->getUsername() + "@localhost PRIVMSG " + it->second.getNickname() + ":" + cmds[2] + "\r\n";
                    send(it->first, Message.c_str(), Message.length(), 0);
                    break;
                }   
            }
            if(!userFound)
            {
                Message = ":localhost 401 " + client->getNickname() + " " + Target +" :No such nick/channel\r\n";
                send(clientFd, Message.c_str(), Message.length(), 0);
            }
        }
    }
}
