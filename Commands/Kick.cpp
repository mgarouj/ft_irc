#include "../Headers/Server.hpp"

void Server::handleKick(int clientFd, std::vector<std::string>& cmds)
{

    if (cmds.size() < 3)
        sendError(clientFd, 461, "KICK");
    else 
    {
        std::string channel = cmds[1];
        std::string nick = cmds[2];
        std::string reason;
        if (cmds.size() > 3)
            reason = cmds[3];
        if (CheckExist(clientFd, channel))
        {
            std::map<std::string, Channel>::iterator it = channels.find(channel);
            std::map<int,Client>::iterator it1 = clients.begin();
            for(it1 = clients.begin(); it1 != clients.end(); ++it1)
            {
                if (it1->second.getNickname() == nick)
                    break;
            }
            if (it1 == clients.end())
                sendError(clientFd, 401);
            else if (!(it->second.isMember(&(it1->second))))
                sendMsg(clientFd, 441, nick + " " + channel + " :They aren't on that channel");
            else
            {
                it->second.broadcastMessage(": " + clients[clientFd].getNickname() + " KICK " + channel + " " + nick + " :" + reason + "\n\r", &clients[clientFd]);
                sendMsg(clientFd, 001, ": " + clients[clientFd].getNickname() + " KICK " + channel + " " + nick + " :" + reason + "\n\r");
                it->second.removeMember(&(it1->second));
                it->second.removeInvited(&(it1->second));
                if(it->second.isEmpty())
                {
                    std::map<std::string, Channel>::iterator it2 = channels.find(it->first);
                    if (it2 != channels.end())
                        channels.erase(it2);
                }
            }
        }
    }
}