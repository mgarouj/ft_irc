#include "../Headers/Client.hpp"

Client::Client() : fd(-1), nickname(""), username(""), realname(""), password(""), authenticated(false) {}

Client::Client(int fd) : fd(fd), nickname(""), username(""), realname(""), password(""), authenticated(false) {}

Client::Client(const Client &other) : fd(other.fd), nickname(other.nickname), username(other.username), realname(other.realname), password(other.password), authenticated(other.authenticated) {}

Client &Client::operator=(const Client &other)
{
    if (this != &other)
    {
        fd = other.fd;
        nickname = other.nickname;
        username = other.username;
        realname = other.realname;
        password = other.password;
        authenticated = other.authenticated;
    }
    return *this;
}

Client::~Client() {}

int Client::getFd() const { return fd; }

const std::string &Client::getNickname() const { return nickname; }

const std::string &Client::getUsername() const { return username; }

const std::string &Client::getRealname() const { return realname; }

bool Client::isAuthenticated() const { return authenticated; }

void Client::setNickname(const std::string &nickname) { this->nickname = nickname; }

void Client::setUsername(const std::string &username) { this->username = username; }

void Client::setRealname(const std::string &realname) { this->realname = realname; }

void Client::setPassword(const std::string &password) { this->password = password; }

void Client::authenticate(const std::string &serverPassword)
{
    if (serverPassword == password)
        passAuthentication = true;
    if (passAuthentication && nickAuthentication && userAuthentication)
        authenticated = true;
}

void Client::setclientBuffer(const std::string &buffer)
{
    clientBuffer += buffer; 
}

std::string &Client::getclientBuffer()
{
    return clientBuffer;
}