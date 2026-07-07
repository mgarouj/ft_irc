#include "../Headers/Server.hpp"

bool Server::validTopic(std::string &channel, int clientFd)
{
    std::map<std::string,Channel>::iterator it = channels.find(channel);
    if (it == channels.end())
    {
        sendError(clientFd, 403, channel);
        return false;
    }
    else
    {
        std::map<int,Client>::iterator it1 = clients.find(clientFd);
        if (it1 != clients.end())
        {
            if (!it->second.isMember(&(it1->second)))
            {
                sendError(clientFd, 442, channel);
                return false;
            }
        }
    }
    return true;
}



void Server::handleTopic(int clientFd, std::vector<std::string>& cmds)
{
    if (cmds.size() < 2)
        sendError(clientFd, 461, "TOPIC");
    else
    {
        std::string channel = cmds[1];
        std::map<std::string, Channel>::iterator it = channels.find(channel);
        std::map<int,Client>::iterator it1 = clients.find(clientFd);
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
                if (it->second.isTopicRestricted() && !(it->second.isOperator(&(it1->second))))
                {
                    std::string message = clients[clientFd].getNickname() + " " + channel + " :You're not channel operator";
                    sendError(clientFd, 482, message);
                }
                else
                {
                    it->second.setTopic(reason);
                    it->second.broadcastMessage(": " + clients[clientFd].getNickname() + " TOPIC " + channel + " :" + it->second.getTopic() + "\r\n", &clients[clientFd]);
                    sendMsg(clientFd, 332, clients[clientFd].getNickname() + " " + channel + " :" + it->second.getTopic());
                }
            }
        }

    }
}