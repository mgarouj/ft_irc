#include "bot.hpp"


Bot::Bot(): _ipServer(""), _port(0), _nickBot(""), _passServer("") {}

Bot::Bot(std::string is, int port, std::string nickBot, std::string passServer): _port(port), _nickBot(nickBot), _passServer(passServer){
    if (is == "localhost")
        this->_ipServer = "127.0.0.1";
    else
        this->_ipServer = is;
}

Bot::Bot(const Bot &other): _ipServer(other._ipServer), _port(other._port), _nickBot(other._nickBot), _passServer(other._passServer){}

Bot::~Bot() {
    if (botsocket >= 0) {
        close(botsocket);
    }
}

Bot &Bot::operator=(const Bot &other) {
    if (this != &other) {
        _ipServer = other._ipServer;
        _port = other._port;
        _nickBot = other._nickBot;
        _passServer = other._passServer;
    }
    return *this;
}

void Bot::botAuthenticate(std::string &authenticate) {

    if (send(botsocket, authenticate.c_str(), authenticate.length(), 0) < 0) {
        throw std::runtime_error("Error: Failed to send authentication.");
    }
    std::string full_response = "";
    char buffer[1024];
        memset(buffer, 0, sizeof(buffer)); 
        int bytes = recv(botsocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            throw std::runtime_error("Error: Server disconnected during authentication.");
        }
        full_response = buffer;
        
        if (full_response.find(" 001 ") != std::string::npos) {
            std::cout << "\n✅ Bot successfully authenticated!" << std::endl;
        }

        if (full_response.find(" 464 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Incorrect Password.");
        
        if (full_response.find(" 433 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Nickname '" + _nickBot + "' is already in use.");
        
        if (full_response.find(" 461 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Not enough parameters.");
}

void Bot::init()
{
    this->botsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->botsocket < 1)
        throw std::runtime_error("Error: Bot failed to create socket.");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->_port);
    serverAddress.sin_addr.s_addr = inet_addr(this->_ipServer.c_str());
}

void Bot::run()
{
    this->init();
    if (connect(botsocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        throw std::runtime_error("Error: Bot failed to connect.");

    std::string authenticate = "PASS " + this->_passServer + "\r\n" + 
                                "NICK " + this->_nickBot + "\r\n" + 
                                "USER " + this->_nickBot + " 0 * :" + this->_nickBot + "\r\n";
    
    botAuthenticate(authenticate);
    listenAndRespond();
}

void Bot::listenAndRespond() {
    char buffer[1024];
    std::string leftover = "";
    
    std::cout << "Bot is now listening for commands (!joke, !ping, !help)..." << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer)); 
        int bytesRead = recv(botsocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesRead <= 0) {
            std::cout << "Server disconnected or error occurred." << std::endl;
            break;
        }

        leftover += buffer;
        size_t pos;
        
        while ((pos = leftover.find("\r\n")) != std::string::npos) {
            std::string line = leftover.substr(0, pos);
            leftover.erase(0, pos + 2);

            std::cout << "[Server] " << line << std::endl;

            if (line.find("PRIVMSG") != std::string::npos) {
                
                size_t excPos = line.find('!');
                if (line[0] == ':' && excPos != std::string::npos) {
                    std::string senderNick = line.substr(1, excPos - 1);

                    if (line.find("!joke") != std::string::npos) {
                        std::string reply = "PRIVMSG " + senderNick + " :Why do C++ programmers prefer dark mode? Because light attracts bugs!\r\n";
                        send(botsocket, reply.c_str(), reply.length(), 0);
                    }
                    else if (line.find("!ping") != std::string::npos) {
                        std::string reply = "PRIVMSG " + senderNick + " :PONG! I am alive!\r\n";
                        send(botsocket, reply.c_str(), reply.length(), 0);
                    }
                    else if (line.find("!help") != std::string::npos) {
                        std::string reply = "PRIVMSG " + senderNick + " :Available commands: !joke, !ping, !help\r\n";
                        send(botsocket, reply.c_str(), reply.length(), 0);
                    }
                }
            }
        }
    }
}