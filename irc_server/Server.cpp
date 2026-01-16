#include "Server.hpp"

Server::Server(int _port, std::string _password):_port(_port), _password(_password){

}

Server::Server(const Server& ob):_port(ob._port), _password(ob._password), _Server_socket_fd(ob._Server_socket_fd), _fds(ob._fds){

}

Server::~Server(){}

Server& Server::operator=(const Server& ob){
	if (this != &ob)
	{
		this->_port = ob._port;
		this->_password = ob._password;
		this->_Server_socket_fd = ob._Server_socket_fd;
		this->_fds = ob._fds;
	}
	return *this;
}

void Server::init(){
	_Server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_Server_socket_fd == -1)
		throw std::runtime_error("Error: Failed to creat socket.");
	
	int en = 1;
	if (setsockopt(_Server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw std::runtime_error("Error: Failed to set socket options.");

	if (fcntl(_Server_socket_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: Failed to set non-blocking mode.");
	
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(_port);

	if (bind(_Server_socket_fd, (struct  sockaddr *)&sa, sizeof(sa)) < 0)
		throw std::runtime_error("Error: Failed to bind socket.");

	if (listen(_Server_socket_fd, SOMAXCONN) < 0)
		throw std::runtime_error("Error: Failed to listen.");

	struct pollfd pfd;
	pfd.fd = _Server_socket_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_fds.push_back(pfd);

	std::cout << "server initialized successfully on port " << _port << std::endl;    
}

// void Server::acceptNewClient(){

// }

// void Server::handleClientData(){

// }

void Server::run(int c, char **v){
	while (true){
		int ret = poll(this->_fds.data(), this->_fds.size(), -1);
		if (ret < 0){
			throw std::runtime_error("poll error");
			break;
		}
	}
}
