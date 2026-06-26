#include "bot.hpp"


// Bot::Bot(){ipServer(""), }




Bot::Bot(std::string is, int port, std::string nickBot, std::string passServer): _ipServer(is), _port(port), _nickBot(nickBot), _passServer(passServer)
{
    this->botsocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->_port);
    serverAddress.sin_addr.s_addr = inet_addr(this->_ipServer.c_str());


    connect(botsocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    std::string authenticate = "PASS " + this->_passServer + "\r\n" + "NICK " + this->_nickBot + "\r\n" + "USER " + this->_nickBot + " 0 * :" + this->_nickBot + "\r\n";
    send(botsocket, authenticate.c_str(), authenticate.length(), 0);
     char check_auth[1024];
    bzero(check_auth, 1024);

    int bytes = recv(botsocket, check_auth, 1023, 0);

    if (bytes > 0)
    {
        std::string response = check_auth;

        if (response.find(" 464 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Incorrect Password.");

        if (response.find(" 433 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Nickname '" + _nickBot + "' is already in use.");

        if (response.find(" 461 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Not enough parameters.");
    }
    
}


int main(int c, char **v)
{   
    // <ip> <port> <nick> <pass> 
    if (c != 5)
    {
        std::cerr << "./bot <IP of server> <Port> <nickname of Bot> <password of server> "  << std::endl;
        return 1;
    }
    int port = std::atoi(v[2]);
    try
    {
        /* code */
        Bot bot(v[1], port, v[3], v[4]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;

}