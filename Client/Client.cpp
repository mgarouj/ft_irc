#include "../Headers/Client.hpp"

#include "../Headers/Client.hpp"

// Default Constructor
Client::Client() : fd(-1), nickname(""), username(""), realname(""), password(""), 
                   authenticated(false), passAuthentication(false), 
                   nickAuthentication(false), userAuthentication(false), channels_counter(0) {}

// Parameterized Constructor
Client::Client(int fd) : fd(fd), nickname(""), username(""), realname(""), password(""), 
                         authenticated(false), passAuthentication(false), 
                         nickAuthentication(false), userAuthentication(false), channels_counter(0) {}

// Copy Constructor
Client::Client(const Client &other) : fd(other.fd), nickname(other.nickname), 
                                      username(other.username), realname(other.realname), 
                                      password(other.password), authenticated(other.authenticated),
                                      passAuthentication(other.passAuthentication), 
                                      nickAuthentication(other.nickAuthentication), 
                                      userAuthentication(other.userAuthentication),
                                      clientBuffer(other.clientBuffer), pairCmd(other.pairCmd) {}

// Assignment Operator
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
        passAuthentication = other.passAuthentication;
        nickAuthentication = other.nickAuthentication;
        userAuthentication = other.userAuthentication;
        
        clientBuffer = other.clientBuffer;
        pairCmd = other.pairCmd;
    }
    return *this;
}

Client::~Client() {}
int Client::getFd() const { return fd; }

const std::string &Client::getNickname() const { return nickname; }

const std::string &Client::getUsername() const { return username; }

const std::string &Client::getRealname() const { return realname; }

const std::string &Client::getHost() const { return host; }

const std::string &Client::getServername() const { return servername; }

const int &Client::getchannels_counter() const { return channels_counter; }

bool Client::isAuthenticated() const { return authenticated; }

void Client::setNickname(const std::string &nickname) { this->nickname = nickname; }

void Client::setUsername(const std::string &username) { this->username = username; }

void Client::setRealname(const std::string &realname) { this->realname = realname; }

void Client::setPassword(const std::string &password) { this->password = password; }

void Client::setHost(const std::string &host) { this->host = host; }

void Client::setServername(const std::string &servername) { this->host = servername; }

void Client::setchannels_counter(const int &Channelupdate) { this->channels_counter = Channelupdate; }

void Client::authenticate()
{
    if (getPassAuthentication() && getNickAuthentication() && getUserAuthentication())
        authenticated = true;
    else 
        authenticated = false;
}

void Client::setclientBuffer(const std::string &buffer)
{
    clientBuffer += buffer; 
}

std::string &Client::getclientBuffer()
{
    return clientBuffer;
}

void Client::setPassAuthentication(bool status)
{
    passAuthentication = status;
}

void Client::setNickAuthentication(bool status)
{
    nickAuthentication = status;
}

void Client::setUserAuthentication(bool status)
{
    userAuthentication = status;
}

bool Client::getPassAuthentication() const
{
    return passAuthentication;
}

bool Client::getNickAuthentication() const
{
    return nickAuthentication;
}

bool Client::getUserAuthentication() const
{
    return userAuthentication;
}
void Client::clearBuffer()
{
    clientBuffer.clear();
}
