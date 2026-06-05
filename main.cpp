#include "Server.hpp"


int main(int c, char **v)
{
    if (c != 3)
    {
        std::cerr << "Usage: " << v[0] << " <port> <password>" << std::endl;
        return 1;
    }
    return 0;
}