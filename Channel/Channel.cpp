#include "../Headers/Channel.hpp"
#include <algorithm> 
#include <sys/socket.h>

Channel::Channel() : name(""), topic(""), password(""), inviteOnly(false), topicRestricted(false), userLimit(0) {}

Channel::Channel(const std::string& name) : name(name), topic(""), password(""), inviteOnly(false), topicRestricted(false), userLimit(0) {}

Channel::~Channel() {}

const std::string& Channel::getName() const 
{ 
    return name; 
}
const std::string& Channel::getTopic() const 
{ 
    return topic; 
}

const std::string& Channel::getPass() const 
{ 
    return password; 
}


bool Channel::HasPass() const 
{ 
    return password != "" ? true : false; 
}


void Channel::setPassword(const std::string& newPassword) 
{ 
    password = newPassword; 
}

void Channel::setTopic(const std::string& newTopic) 
{ 
    topic = newTopic; 
}

void Channel::setinviteOnlyattr(bool is)
{
    inviteOnly = is;
}

void Channel::setTopicRestricted(bool isRestricted)
{
    topicRestricted = isRestricted;
}

void Channel::setUserLimit(size_t limit)
{
    userLimit = limit;
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

void Channel::removeOperator(Client* client)
{
    for(size_t i = 0; i < operators.size(); ++i)
    {
        if(client->getFd() == operators[i]->getFd())
        {
            operators.erase(operators.begin() + i);
        }
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

bool Channel::isInviteOnly() const 
{
    return inviteOnly;
}

bool Channel::isChannelFull() const
{
    return (userLimit == 0 ? false : members.size() >= userLimit);
}

bool Channel::isTopicRestricted() const
{
    return topicRestricted;
}

bool Channel::isEmpty() const
{
    return (members.empty());
}

void Channel::sendNamesList(int clientFd, const std::string& clientNick)
{
    std::string prefix = ":ircserv.com 353 " + clientNick + " = " + this->name + " :";
    std::string currentLine = prefix;

    for (size_t i = 0; i < members.size(); ++i)
    {
        std::string userEntry = "";
        
        if (isOperator(members[i]))
            userEntry += "@";
            
        userEntry += members[i]->getNickname();

        if (currentLine.length() + userEntry.length() + 1 > 510)
        {
            currentLine += "\r\n";
            send(clientFd, currentLine.c_str(), currentLine.length(), 0);
            currentLine = prefix; 
        }
        currentLine += userEntry + " ";
    }

    if (currentLine.length() > prefix.length())
    {
        currentLine += "\r\n";
        send(clientFd, currentLine.c_str(), currentLine.length(), 0);
    }
    std::string endMsg = ":ircserv.com 366 " + clientNick + " " + this->name + " :End of /NAMES list\r\n";
    send(clientFd, endMsg.c_str(), endMsg.length(), 0);
}

void Channel::broadcastMessage(const std::string& message, Client* sender) 
{
    for (size_t i = 0; i < members.size(); ++i) 
    {
        if (sender == NULL || members[i]->getFd() != sender->getFd()) 
        {
            send(members[i]->getFd(), message.c_str(), message.length(), 0);
        }
    }
}

void Channel::addInvited(Client* client) 
{
    if (!isInvited(client)) 
    {
        guests.push_back(client);
    }
}

void Channel::removeInvited(Client* client) 
{
    std::vector<Client*>::iterator it = std::find(guests.begin(), guests.end(), client);
    if (it != guests.end()) 
    {
        guests.erase(it);
    }

}

bool Channel::isInvited(Client* client) const 
{
    return std::find(guests.begin(), guests.end(), client) != guests.end();
}
