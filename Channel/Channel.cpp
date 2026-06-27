#include "../Headers/Channel.hpp"
#include <algorithm> 
#include <sys/socket.h>

Channel::Channel(const std::string& name) : name(name), topic(""), password("") {}

Channel::~Channel() {}

const std::string& Channel::getName() const 
{ 
    return name; 
}
const std::string& Channel::getTopic() const 
{ 
    return topic; 
}

bool Channel::isRestricted() const
{
    return topicRestricted;
}
void Channel::setTopic(const std::string& newTopic) 
{ 
    topic = newTopic; 
}

void Channel::addMember(Client* client) 
{
    if (!isMember(client)) 
    {
        members.push_back(client);
    }
}

void Channel::removeMember(Client* client) 
{
    std::vector<Client*>::iterator it = std::find(members.begin(), members.end(), client);
    if (it != members.end()) 
    {
        members.erase(it);
    }

    std::vector<Client*>::iterator opIt = std::find(operators.begin(), operators.end(), client);
    if (opIt != operators.end()) 
    {
        operators.erase(opIt);
    }
}

void Channel::addOperator(Client* client) 
{
    if (!isOperator(client)) 
    {
        operators.push_back(client);
    }
}

bool Channel::isMember(Client* client) const 
{
    return std::find(members.begin(), members.end(), client) != members.end();
}

bool Channel::isOperator(Client* client) const 
{
    return std::find(operators.begin(), operators.end(), client) != operators.end();
}

void Channel::broadcastMessage(const std::string& message, Client* sender) 
{
    for (size_t i = 0; i < members.size(); ++i) 
    {
        // If sender is provided, don't echo the message back to them (useful for PRIVMSG)
        // If sender is NULL, send to everyone (useful for JOIN, KICK, PART, TOPIC)
        if (sender == NULL || members[i]->getFd() != sender->getFd()) 
        {
            send(members[i]->getFd(), message.c_str(), message.length(), 0);
        }
    }
}
