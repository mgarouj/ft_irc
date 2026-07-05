#include "../Headers/Server.hpp"

void Server::handlePart(int clientFd, std::vector<std::string>& cmds)
{
    Client* client = &clients[clientFd];
    std::string Message = "";
    if (cmds.size() == 1) {
        sendError(clientFd, 461, "PART");
        return;
    }
    for (std::map<std::string, Channel>::iterator ite = channels.begin(); ite != channels.end(); ite++)
    {
        if (ite->second.isMember(client))
        {
            Message = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + " PART " + ite->first + "\r\n";
            ite->second.broadcastMessage(Message, client);
            ite->second.removeMember(client);
            send(clientFd, Message.c_str(), Message.length(), 0);
            if(client->getchannels_counter() >= 0)
                client->setchannels_counter(client->getchannels_counter() - 1);
            if(ite->second.isEmpty())
            {
                std::map<std::string, Channel>::iterator it = channels.find(ite->first);
                if (it != channels.end())
                    channels.erase(it);
                break;
            }
        }
    }
}
