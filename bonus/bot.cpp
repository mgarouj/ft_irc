#include "bot.hpp"


// Bot::Bot(){ipServer(""), }




Bot::Bot(std::string is, int port, std::string nickBot, std::string passServer): _ipServer(is), _port(port), _nickBot(nickBot), _passServer(passServer)
{
    this->botsocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->_port);
    serverAddress.sin_addr.s_addr = inet_addr(this->_ipServer.c_str());

}
void Bot::botAuthenticate(std::string &authenticate) {
    // Send the authentication string
    if (send(botsocket, authenticate.c_str(), authenticate.length(), 0) < 0) {
        throw std::runtime_error("Error: Failed to send authentication.");
    }
    
    std::string full_response = "";
    char buffer[1024];
    
    std::cout << "Waiting for server response..." << std::endl;
    
    // The Wait and Accumulate Loop
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        
        int bytes = recv(botsocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes <= 0) {
            throw std::runtime_error("Error: Server disconnected during authentication.");
        }
        
        std::cout << " buffer : " << std::endl;
        full_response += buffer;
        std::cout << "Server says: " << buffer; // Print to terminal for debugging

        // Check for SUCCESS (001 RPL_WELCOME)
        if (full_response.find(" 001 ") != std::string::npos) {
            std::cout << "\n✅ Bot successfully authenticated!" << std::endl;
            break; // Authentication complete, exit the loop
        }

        // Check for ERRORS
        if (full_response.find(" 464 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Incorrect Password.");
        
        if (full_response.find(" 433 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Nickname '" + _nickBot + "' is already in use.");
        
        if (full_response.find(" 461 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Not enough parameters.");
    }
}

void Bot::run() {
    if (connect(botsocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        throw std::runtime_error("Error: Bot failed to connect.");
    }
    std::cout << "Connected to server successfully!" << std::endl;

std::string authenticate = "PASS " + this->_passServer + "\r\n" + 
                               "NICK " + this->_nickBot + "\r\n" + 
                               "USER " + this->_nickBot + " 0 * :" + this->_nickBot + "\r\n";
    
    botAuthenticate(authenticate);
}