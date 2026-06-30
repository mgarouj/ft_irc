#include "bot.hpp"
int main(int c, char **v)
{   
    if (c != 5)
    {
        std::cerr << "./bot <IP of server> <Port> <nickname of Bot> <password of server> "  << std::endl;
        return 1;
    }
    int port = std::atoi(v[2]);
    try
    {
        Bot bot(v[1], port, v[3], v[4]);
        bot.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}