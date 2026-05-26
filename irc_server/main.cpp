#include "Server.hpp"

void isValidePsPo(char **v)
{
    std::string _port(v[1]); // add port v[1] from argv
    std::string _password(v[2]); // add password v[2] from argv

    for (size_t i = 0; i < _port.length(); i++)
    {
        if (!std::isdigit(_port[i])) // check if port contain anything else 
            throw std::runtime_error("Error: Invalide port!"); // throw exeption if any error exist
    }

    int portInt = std::atoi(_port.c_str()); // convert port to int 
    if (portInt < 1024 || portInt > 65535) 
        throw std::runtime_error("Error: (Use range 1024-65535)"); // if port portInt < 1024 || portInt > 65535 -> this range 
    
    if (_password.empty()) // if password not exit should throw exeption 
        throw std::runtime_error("Error: password cannot be empty");
}

int main(int c, char **v)
{
    if (c != 3) // if c != 3 we should print error in std::err
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