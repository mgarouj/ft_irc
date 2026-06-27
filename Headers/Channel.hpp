#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class Channel {
    private:
        std::string name;
        std::string topic;
        std::string password; // For +k mode
        std::vector<Client*> members;
        std::vector<Client*> operators;
        bool topicRestricted;
        //booleans for modes inviteOnly (+i), topicRestricted (+t), etc.....

    public:
        Channel(const std::string& name);
        ~Channel();

        const std::string& getName() const;
        const std::string& getTopic() const;
        bool isRestricted() const;/////
        void setTopic(const std::string& newTopic);

        void addMember(Client* client);
        void removeMember(Client* client);
        void addOperator(Client* client);
        
        bool isMember(Client* client) const;
        bool isOperator(Client* client) const;
        
        void broadcastMessage(const std::string& message, Client* sender);
};

#endif
