#include "Headers/Server.hpp"

void isValidParameter(int c, char **v) 
{
    if (c != 3) 
        throw std::invalid_argument("Usage: ./ircserv <port> <password>");
    std::string portStr(v[1]);
    if (portStr.empty())
        throw std::invalid_argument("Error: Port cannot be empty.");
    for (size_t i = 0; i < portStr.length(); ++i) 
        if (!std::isdigit(portStr[i])) 
            throw std::invalid_argument("Error: Port must contain only digits.");
    
    int port = std::atoi(portStr.c_str());
    if (port < 1 || port > 65535) 
        throw std::invalid_argument("Error: Port must be between 1 and 65535.");

    std::string password(v[2]);
    if (password.empty()) 
        throw std::invalid_argument("Error: Password cannot be empty.");

    for (size_t i = 0; i < password.length(); ++i) 
        if (std::isspace(password[i]) || !std::isprint(password[i])) 
            throw std::invalid_argument("Error: Password contains invalid characters (spaces and control chars are not allowed).");
}

int main(int c, char **v)
{
    try
    {
        isValidParameter(c, v);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    Server s(v[2], std::atoi(v[1]));
    try
    {
        signal(SIGPIPE, SIG_IGN);
        signal(SIGINT, s.signalR);
        signal(SIGQUIT, s.signalR);
        s.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}