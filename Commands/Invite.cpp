#include "../Headers/Server.hpp"

bool Server::CheckExist(int clientFd, std::string &channel)
{
    std::map<std::string, Channel>::iterator it = channels.find(channel);
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
            else if (!it->second.isOperator(&(it1->second)))
            {
                sendMsg(clientFd, 482, channel + " ::You're not channel operator");/// make sure
                return false;
            }
        }
    }
    return true;
}

bool Server::CheckNew(std::string &nick, std::string &channel, int clientFd)
{
    std::map<int, Client>::iterator it;
    for(it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.getNickname() == nick)
            return true;
    }
    if (it == clients.end())
        sendMsg(clientFd, 401, nick + " :No such nick/channel");
    else
    {
        std::map<std::string, Channel>::iterator it1 = channels.find(channel);
        if (it1 != channels.end())
        {
            sendMsg(clientFd, 443, nick + " " + channel + " :is already on channel");
            return false;
        }
    }
    return true;
}

void Server::handleInvite(int clientFd, std::vector<std::string>& cmds)
{
    if (cmds.size() < 2)
        sendMsg(clientFd, 461, " INVITE :Not enough parameters");
    else 
    {
        std::string New = cmds[1];
        std::string channel = cmds[2];
        if (CheckExist(clientFd, channel) && CheckNew(New, channel, clientFd))
        {
            std::map<int, Client>::iterator it = clients.begin();
            std::map<std::string, Channel>::iterator it1 = channels.find(channel);
            for (it = clients.begin(); it != clients.end(); ++it)
            {
                if (it->second.getNickname() == New)
                    break;
            }
            sendMsg(it->first,'\0',":" + clients[clientFd].getNickname() + " INVITE " + New + " :" + channel);
            sendMsg(clientFd, 341, New + channel);
            it1->second.addMember(&(it->second));
        }
    }
}
