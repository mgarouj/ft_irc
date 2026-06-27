#include "../Headers/Server.hpp"
#include "../Headers/Client.hpp"
#include "../Headers/Channel.hpp"

std::vector<std::string> split_all(std::string &cmd)
{
    std::vector<std::string> args;
    std::stringstream ss(cmd);

    std::string token;

    while (ss >> token)
    {
        if (token[0] == ':')
        {
            std::string rest;
            std::getline(ss, rest);
            token.erase(0, 1);
            token += rest;
            args.push_back(token);
            break;
        }
        args.push_back(token);
    }
    return args;
}

void sendMsg(int fd, int code, const std::string& msg)
{
    std::stringstream ss;

    ss << "ircserv " << code << " " << msg << "\r\n";
    send(fd, ss.str().c_str(), ss.str().size(), 0);
}

// int checkPoint(std::string &str)
// {
//     for (int i = 0; i < str.size();i++)
//     {
//         if (str[i] = ':')
//             return 1;
//         return 0;
//     }
// }