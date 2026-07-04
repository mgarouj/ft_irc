#include "../Headers/Server.hpp"



std::string Server::getErrorMsg(int errCode, const std::string& detail) 
{
    switch (errCode) 
    {
        case 401: return detail + " :No such nick/channel";
        case 403: return detail + " :No such channel";
        case 404: return detail + " :Cannot send to channel";
        case 411: return ":No recipient given (" + detail + ")";
        case 412: return ":No text to send";
        case 421: return detail + " :Unknown command";
        case 431: return ":No nickname given";
        case 432: return detail + " :Erroneus nickname";
        case 433: return detail + " :Nickname is already in use";
        case 441: return detail + " :They aren't on that channel"; // detail = "<nick> <channel>"
        case 442: return detail + " :You're not on that channel";
        case 451: return ":You have not registered";
        case 461: return detail + " :Not enough parameters";
        case 462: return ":You may not reregister";
        case 464: return ":Password incorrect";
        case 471: return detail + " :Cannot join channel (+l)";
        case 473: return detail + " :Cannot join channel (+i)";
        case 474: return detail + " :Cannot join channel (+b)";
        case 475: return detail + " :Cannot join channel (+k)";
        case 481: return ":Permission Denied- You're not an IRC operator";
        case 482: return detail + " :You're not channel operator";
        
        
        default:  return ":Unknown error";
    }
}

void Server::sendError(int clientFd, int errCode, const std::string& detail) 
{
    std::ostringstream oss;
    
    std::string nick = clients[clientFd].getNickname();
    if (nick.empty()) {
        nick = "*";
    }

    oss << ":" << this->serverName << " " << errCode << " " << nick << " " << getErrorMsg(errCode, detail) << "\r\n";

    std::string response = oss.str();
    send(clientFd, response.c_str(), response.length(), 0);
}