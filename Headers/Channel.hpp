#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class Channel {
    private:
        std::string name;
        std::string topic;
        std::string password;
        std::vector<Client*> members;
        std::vector<Client*> operators;
        std::vector<std::string> bannedUsers;
        bool inviteOnly;
        bool topicRestricted;
        size_t userLimit;

    public:
        Channel();
        Channel(const std::string& name);
        ~Channel();

        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getPass() const;
        bool HasPass() const;
        void setPassword(const std::string& newPassword);
        void setTopic(const std::string& newTopic);
        void setinviteOnlyattr(bool is);
        void setTopicRestricted(bool isRestricted);
        void setUserLimit(size_t limit);

        void addMember(Client* client);
        void removeMember(Client* client);
        void addOperator(Client* client);
        
        bool isMember(Client* client) const;
        bool isOperator(Client* client) const;
        bool isBanned(Client* client) const;
        bool isInviteOnly() const;
        bool isChannelFull() const;
        bool isTopicRestricted() const;
        
        void sendNamesList(int clientFd, const std::string& clientNick);
        void broadcastMessage(const std::string& message, Client* sender);
        void removeOperator(Client* client);
};

#endif
