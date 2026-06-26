#include "../Headers/Server.hpp"

void Server::handleJoin(int clientFd, std::vector<std::string>& cmds)
{
    Client* client = &clients[clientFd];
    std::string Message = "";
    // JOIN
    if(cmds.size() == 1)
    {
        Message = ":461 " + client->getNickname() + " :Not enough parameters\r\n";
        send(clientFd, Message.c_str(), Message.length(), 0);
        return;
    }
    // JOIN 0
    if(cmds.size() == 2 && cmds[1] == "0")
    {
        for(std::map<std::string, Channel>::iterator ite = channels.begin(); ite != channels.end(); ite++)
        {
            if(ite->second.isMember(client))
            {
                Message = ":" + client->getNickname() + " PART " + ite->first + "\r\n";
                send(clientFd, Message.c_str(), Message.length(), 0);
                ite->second.removeMember(client);
                ite->second.broadcastMessage(Message, NULL);
            }
        }
        return;
    }
    std::vector<std::string> tempChannels;
    std::vector<std::string> tempKeys;
    std::stringstream ssChannels(cmds[1]);
    std::string token;

    //store the channels
    while (std::getline(ssChannels, token, ',')) 
    {
        tempChannels.push_back(token);
    }

    //store the keys of channels
    if (cmds.size() > 2) 
    {
        std::stringstream ssKeys(cmds[2]);
        while (std::getline(ssKeys, token, ',')) 
        {
            tempKeys.push_back(token);
        }
    }
    for (size_t i = 0; i < tempChannels.size(); i++)
    {
        std::string currentChan = tempChannels[i];
        std::string currentKey = "";
        if (i < tempKeys.size()) {
            currentKey = tempKeys[i];
        }

        if (currentChan.empty() || (currentChan[0] != '#' && currentChan[0] != '&'))
        {
            Message = ":localhost 403 " + client->getNickname() + " " + currentChan + " :No such channel\r\n";
            send(clientFd, Message.c_str(), Message.length(), 0);
            continue;
        }

        if(client->getchannels_counter() == 30)
        {
            Message = ":405 " + client->getNickname() + " : You Have been reach the limit of the channel joining\r\n";
            send(clientFd, Message.c_str(), Message.length(), 0);
            return;
        }

        for(std::map<std::string, Channel>::iterator ite = channels.begin(); ite != channels.end(); ite++)
        {
            if(ite->first == currentChan)
            {
                if(ite->second.isBanned(client))
                {
                    Message = ":474 " + client->getNickname() + " " + ite->second.getName() + ":Cannot join channel (+b)\r\n";
                    send(clientFd, Message.c_str(), Message.length(), 0);
                    return;
                }
                else if(ite->second.isInviteOnly())
                {
                    Message = ":473 " + client->getNickname() + " " + ite->second.getName() + ":Cannot join channel (+i)\r\n";
                    send(clientFd, Message.c_str(), Message.length(), 0);
                    return;
                }
                else
                {
                    if(ite->second.isChannelFull())
                    {
                        Message = ":471 " + client->getNickname() + " " + ite->second.getName() + " :Cannot join channel (+l)\r\n";
                        send(clientFd, Message.c_str(), Message.length(), 0);
                        return;
                    }
                    if(ite->second.HasPass())
                    {
                        if(ite->second.getPass() != currentKey)
                        {
                            Message = ":475 " + client->getNickname() + " " + ite->second.getName() + " :Cannot join channel (+k)\r\n";
                            send(clientFd, Message.c_str(), Message.length(), 0);
                            return;
                        }
                        else
                        {
                            ite->second.addMember(client);
                            Message = client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + " JOIN :" + ite->second.getName();
                            ite->second.broadcastMessage(Message, NULL);
                            if(ite->second.getTopic() != "")
                            {
                                Message = ":332 " + client->getNickname() + " " + ite->second.getName() + " :" + ite->second.getTopic();
                                send(clientFd, Message.c_str(), Message.length(), 0);
                            }
                            channels[currentChan].sendNamesList(clientFd, client->getNickname());
                            return;
                        }
            
                    }
                    else
                    {
                        ite->second.addMember(client);
                        Message = client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + " JOIN :" + ite->second.getName();
                        ite->second.broadcastMessage(Message, NULL);
                        if(ite->second.getTopic() != "")
                        {
                            Message = ":332 " + client->getNickname() + " " + ite->second.getName() + " :" + ite->second.getTopic();
                            send(clientFd, Message.c_str(), Message.length(), 0);
                        }
                        channels[currentChan].sendNamesList(clientFd, client->getNickname());
                        return;
                    }
                }
            }
        }
        channels.insert(std::make_pair(currentChan, Channel(currentChan)));
        Channel& newChannel = channels.find(currentChan)->second;
        newChannel.addMember(client);
        newChannel.addOperator(client);
        Message = ":" + client->getNickname() + " JOIN :" + currentChan + "\r\n";
        send(clientFd, Message.c_str(), Message.length(), 0);
        newChannel.sendNamesList(clientFd, client->getNickname());
        continue; 
    }
}