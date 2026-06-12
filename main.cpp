#include "Headers/Server.hpp"


int main(int c, char **v)
{
    if (c != 3)
    {
        std::cerr << "Usage: " << v[0] << " <port> <password>" << std::endl;
        return 1;
    }
    int port = std::atoi(v[1]);
    if (port <= 0 || port > 65535)
    {
        std::cerr << "Error: Invalid port number. Please provide a port number between 1 and 65535." << std::endl;
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