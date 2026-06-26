#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <fcntl.h>


class Client
{
    private:
        int fd;
        std::string nickname;
        std::string username;
        std::string realname;
        std::string host;
        std::string servername;
        std::string password;

        bool authenticated;
        bool passAuthentication;
        bool nickAuthentication;
        bool userAuthentication;

        std::string clientBuffer;

        std::vector<std::string> pairCmd;
        int channels_counter;

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
        const std::string &getHost() const;
        const std::string &getServername() const;
        const int &getchannels_counter() const;

        std::string &getclientBuffer();
        bool isAuthenticated() const;
        
        bool getPassAuthentication() const;
        bool getNickAuthentication() const;
        bool getUserAuthentication() const;

        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        void setRealname(const std::string &realname);
        void setPassword(const std::string &password);
        void setHost(const std::string &host);
        void setServername(const std::string &servername);
        void setchannels_counter(const int &Channelupdate);
        void setclientBuffer(const std::string &buffer);
        void clearBuffer();
        void authenticate();
        
        void setPassAuthentication(bool status);
        void setNickAuthentication(bool status);
        void setUserAuthentication(bool status);

        

};

#endif