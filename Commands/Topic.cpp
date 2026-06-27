#include "../Headers/Server.hpp"

bool Server::validTopic(std::string &channel, int clientFd)
{
    std::map<std::string,Channel>::iterator it = channels.find(channel);
    if (it == channels.end())
    {
        sendMsg(clientFd, 403, " :No such channel");
        return false;
    }
    else
    {
        std::map<int,Client>::iterator it1 = clients.find(clientFd);
        if (it1 != clients.end())
        {
            if (!it->second.isMember(&(it1->second)))
            {
                sendMsg(clientFd, 442, channel + " :You're not on that channel");
                return false;
            }
        }
    }
    return true;
}



void Server::handleTopic(int clientFd, std::vector<std::string>& cmds)
{
    if (cmds.size() < 3)
        sendMsg(clientFd, 461, " TOPIC :Not enough parameters");
    else
    {
        std::string channel = cmds[1];
        std::map<std::string, Channel>::iterator it = channels.find(channel);
        if (validTopic(channel, clientFd))
        {
            if (cmds.size() == 2)
            {
                if (it->second.getTopic().empty())
                    sendMsg(clientFd, 331, clients[clientFd].getNickname() + " " + channel + " :No topic is set");
                else
                    sendMsg(clientFd, 332, clients[clientFd].getNickname() + " " + channel + " :" + it->second.getTopic());
            }
            else
            {
                std::string reason = cmds[2];
                if (it->second.isRestricted())
                    sendMsg(clientFd, 482, clients[clientFd].getNickname() + " " + channel + " :You're not channel operator");
                else
                {
                    it->second.setTopic(reason);
                    sendMsg(clientFd, 332, clients[clientFd].getNickname() + " " + channel + " :" + it->second.getTopic());
                }
            }
        }

    }
}