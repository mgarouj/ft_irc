#ifndef BOT_HPP
#define BOT_HPP
#include <cstring>
#include <iostream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <map>
#include <signal.h>
#include <arpa/inet.h>

class Bot{
    private:
        std::string _ipServer;
        int     _port;
        std::string _nickBot;
        std::string _passServer;
        int botsocket;
        sockaddr_in serverAddress;
    public:
        Bot();
        Bot(std::string is, int port, std::string nickBot, std::string passServer);
        Bot(const Bot &other);
        ~Bot();
        Bot &operator=(const Bot &other);

        void run();
        void init();
        void botAuthenticate(std::string &authenticate);
        void listenAndRespond();
};

#endif