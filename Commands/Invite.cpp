#include "../Headers/Server.hpp"

void sendMsg(int fd, int code, const std::string& msg)
{
    std::stringstream ss;

    ss << "ircserv " << code << " " << msg << "\r\n";
    send(fd, ss.str().c_str(), ss.str().size(), 0);
}

bool Server::CheckExist(int clientFd, std::string &channel)
{
    std::map<std::string, Channel>::iterator it = channels.find(channel);
    if (it == channels.end())
    {
        sendError(clientFd, 403);
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
            else if (!it->second.isOperator(&(it1->second)))
            {
                sendError(clientFd, 482, channel);
                return false;
            }
        }
    }
    return true;
}

bool Server::CheckNew(std::string &nick, std::string &channel, int clientFd)
{
    std::map<int, Client>::iterator it;
    std::map<std::string, Channel>::iterator it1 = channels.find(channel);
    for(it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.getNickname() == nick)
            break;
    }
    if (it == clients.end())
    {
        sendError(clientFd, 401, nick);
        return false;
    }
    else
    {
        if (it1->second.isMember(&(it->second)))
        {
            sendMsg(clientFd, 443, nick + " " + channel + " :is already on channel");
            return false;
        }
    }
    return true;
}

void Server::handleInvite(int clientFd, std::vector<std::string>& cmds)
{
    if (cmds.size() < 3)
        sendError(clientFd, 461, "INVITE");
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
            it1->second.addInvited(&(it->second));
            sendMsg(it->first,001,":" + clients[clientFd].getNickname() + " INVITE " + it->second.getNickname() + " :" + channel);////
            sendMsg(clientFd, 341, " " +  New + " " + channel);
        }
    }
}
