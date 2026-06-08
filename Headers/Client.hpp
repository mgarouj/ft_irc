#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <fcntl.h>

#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
    private:
        int fd;
        std::string nickname;
        std::string username;
        std::string realname;
        std::string password;

        bool authenticated;
        bool passAuthentication;
        bool nickAuthentication;
        bool userAuthentication;

        std::string clientBuffer;

        std::pair<std::string, std::string> pairCmd;

    public:
        Client();
        Client(int fd);
        Client(const Client &other);
        Client &operator=(const Client &other);
        ~Client();

        int getFd() const;
        const std::string &getNickname() const;
        const std::string &getUsername() const;
        const std::string &getRealname() const;
        bool isAuthenticated() const;

        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        void setRealname(const std::string &realname);
        void setPassword(const std::string &password);
        void setclientBuffer(const std::string &buffer);
        void authenticate(const std::string &serverPassword);
        


};

#endif