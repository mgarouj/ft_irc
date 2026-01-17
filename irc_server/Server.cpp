#include "Server.hpp"

Server::Server(int _port, std::string _password):_port(_port), _password(_password){

}

Server::Server(const Server& ob):_port(ob._port), _password(ob._password), _Server_socket_fd(ob._Server_socket_fd), _fds(ob._fds){

}

Server::~Server(){
	close(this->_Server_socket_fd);
}

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

void Server::acceptNewClient(){
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(_Server_socket_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0){
		perror("accept failed");
		return;
	}

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0){
		perror("fcntl failed");
		close(client_fd);
		return;
	}

	struct pollfd client_pfd;
	client_pfd.fd = client_fd;
	client_pfd.events = POLLIN;
	client_pfd.revents = 0;
	_fds.push_back(client_pfd);

	std::cout << "New client connected! FD: " << client_fd << std::endl;
}

void Server::handleClientData(size_t& i){
	char buffer[1024];

	std::memset(buffer, 0, sizeof(buffer));
	int bytes = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0){
		std::cout << "Client Disconnected. FD: " << _fds[i].fd << std::endl;
		close(_fds[i].fd);
		_fds.erase(_fds.begin() + i);
		i--;
	}
	else{
		std::cout << "Message from " << _fds[i].fd << ": " << buffer << std::endl;
	}
}

void Server::run(){
	while (true){
		if (poll(_fds.data(), _fds.size(), -1) < 0)
			throw std::runtime_error("Error: poll failed");
	

		for (size_t i = 0; i < _fds.size(); i++){
			if (_fds[i].revents & POLLIN){
				if (_fds[i].fd == _Server_socket_fd){
					acceptNewClient();
				}

				else{
					handleClientData(i);
				}
			}
		}
	}
}
