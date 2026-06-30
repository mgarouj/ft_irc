#include "../Headers/Server.hpp"

void Server::handleJoin(int clientFd, std::vector<std::string>& cmds)
{
    Client* client = &clients[clientFd];
    std::string Message = "";

    if (cmds.size() == 1) {
        Message = ":localhost 461 " + client->getNickname() + " JOIN :Not enough parameters\r\n";
        send(clientFd, Message.c_str(), Message.length(), 0);
        return;
    }

    if (cmds.size() == 2 && cmds[1] == "0") {
        for (std::map<std::string, Channel>::iterator ite = channels.begin(); ite != channels.end(); ite++) {
            if (ite->second.isMember(client)) {
                Message = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + " PART " + ite->first + "\r\n";
                ite->second.broadcastMessage(Message, NULL);
                ite->second.removeMember(client);
                send(clientFd, Message.c_str(), Message.length(), 0);
                if(client->getchannels_counter() >= 0)
                    client->setchannels_counter(client->getchannels_counter() - 1);
            }
        }
        return;
    }

    std::vector<std::string> tempChannels;
    std::vector<std::string> tempKeys;
    std::string token;

    std::stringstream ssChannels(cmds[1]);
    while (std::getline(ssChannels, token, ',')) {
        tempChannels.push_back(token);
    }

    if (cmds.size() > 2) {
        std::stringstream ssKeys(cmds[2]);
        while (std::getline(ssKeys, token, ',')) {
            tempKeys.push_back(token);
        }
    }

    for (size_t i = 0; i < tempChannels.size(); i++)
    {
        std::string currentChan = tempChannels[i];
        std::string currentKey = (i < tempKeys.size()) ? tempKeys[i] : "";

        if (currentChan.empty() || (currentChan[0] != '#' && currentChan[0] != '&'))
        {
            Message = ":localhost 403 " + client->getNickname() + " " + currentChan + " :No such channel\r\n";
            send(clientFd, Message.c_str(), Message.length(), 0);
            continue; 
        }

        if (client->getchannels_counter() >= 30)
        {
            Message = ":localhost 405 " + client->getNickname() + " " + currentChan + " :You have reached the channel limit\r\n";
            send(clientFd, Message.c_str(), Message.length(), 0);
            continue; 
        }

        std::string clientPrefix = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHost();
        std::map<std::string, Channel>::iterator ite = channels.find(currentChan);
        
        if (ite != channels.end())
        {
            Channel& channel = ite->second;
            if (channel.isBanned(client))
            {
                Message = ":localhost 474 " + client->getNickname() + " " + channel.getName() + " :Cannot join channel (+b)\r\n";
                send(clientFd, Message.c_str(), Message.length(), 0);
                continue;
            }
            if (channel.isInviteOnly())
            {
                Message = ":localhost 473 " + client->getNickname() + " " + channel.getName() + " :Cannot join channel (+i)\r\n";
                send(clientFd, Message.c_str(), Message.length(), 0);
                continue;
            }
            if (channel.isChannelFull())
            {
                Message = ":localhost 471 " + client->getNickname() + " " + channel.getName() + " :Cannot join channel (+l)\r\n";
                send(clientFd, Message.c_str(), Message.length(), 0);
                continue;
            }
            if (channel.HasPass() && channel.getPass() != currentKey)
            {
                Message = ":localhost 475 " + client->getNickname() + " " + channel.getName() + " :Cannot join channel (+k)\r\n";
                send(clientFd, Message.c_str(), Message.length(), 0);
                continue;
            }
            if(channel.isMember(client))
                return;
            channel.addMember(client);
            Message = clientPrefix + " JOIN :" + channel.getName() + "\r\n";
            channel.broadcastMessage(Message, NULL);
            client->setchannels_counter(client->getchannels_counter() + 1);

            if (channel.isTopicRestricted())
            {
                Message = ":localhost 332 " + client->getNickname() + " " + channel.getName() + " :" + channel.getTopic() + "\r\n";
                send(clientFd, Message.c_str(), Message.length(), 0);
            }
            channel.sendNamesList(clientFd, client->getNickname());
        } 
        else
        {
            channels.insert(std::make_pair(currentChan, Channel(currentChan)));
            Channel& newChannel = channels.find(currentChan)->second;
            
            newChannel.addMember(client);
            newChannel.addOperator(client);
            
            Message = clientPrefix + " JOIN :" + currentChan + "\r\n";
            send(clientFd, Message.c_str(), Message.length(), 0);
            client->setchannels_counter(client->getchannels_counter() + 1);

            newChannel.sendNamesList(clientFd, client->getNickname());
        }
    }
}