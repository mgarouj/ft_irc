#include "Headers/Server.hpp"


int main(int c, char **v)
{
    if (c != 3)
    {
        std::cerr << "Usage: " << v[0] << " <port> <password>" << std::endl;
        return 1;
    }
    Server s(v[2], std::atoi(v[1]));
    try
    {
        s.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}