#include "Server.hpp"

void isValidePsPo(char **v)
{
    std::string _port(v[1]);
    std::string _password(v[2]);

    for (size_t i = 0; i < _port.length(); i++)
    {
        if (!std::isdigit(_port[i]))
            throw std::runtime_error("Error: Invalide port!");
    }

    int portInt = std::atoi(_port.c_str());
    if (portInt < 1024 || portInt > 65535)
        throw std::runtime_error("Error: (Use range 1024-65535)");
    
    if (_password.empty())
        throw std::runtime_error("Error: password cannot be empty");
}

int main(int c, char **v)
{
    if (c != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    try{
        isValidePsPo(v);
    }
    catch (std::exception& ob){
        std::cout << ob.what() << std::endl;
        return 2;
    }

    try{
        Server srv(std::atoi(v[1]), v[2]);
        srv.init();
        srv.run();
    }
    catch (const std::exception& ob)
    {
        std::cout << "Server error: " << ob.what() << std::endl;
    }
    
}