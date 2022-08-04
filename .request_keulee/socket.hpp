#pragma once

// Socket class
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <vector>
#include <cstdlib>

// socket && epoll header
#include <sys/unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define LISTEN_BACKLOG 15
#define DEFAULT -1
#define ERROR -1
#define OK 0

# define YELLOW "\033[0;38;5;220m"
# define GREEN "\033[0;38;5;42m"
# define RED "\033[0;38;5;196m"
# define BLUE "\033[0;38;5;21m"
# define PINK "\033[0;38;5;205m"
# define PURPLE "\033[0;38;5;93m"
# define ORANGE "\033[38;5;202m"
# define FIN "\033[0m"


// Test Block Class
class Block
{
private:
	int socketFd_;
	sockaddr_in sockAddr_;
	int	portNum_;

public:

	Block() : socketFd_(-1)
	{};
	Block(int port) : socketFd_(-1), portNum_(port) {
		memset(&sockAddr_, 0, sizeof(sockAddr_));
	};

	Block(const Block &other) : socketFd_(other.socketFd_), sockAddr_(other.sockAddr_), portNum_(other.portNum_)
	{
		*this = other;
	}
	~Block() {};

	int getter_socketFd() { return this->socketFd_; };
	int getter_portNumber() { return this->portNum_; };
	void setter_socketFd(int fd) { this->socketFd_ = fd; }
	void setter_socketaddr(sockaddr_in tmp) { this->sockAddr_ = tmp; }

	// test function
	void	test_block()
	{
		// std::cout << "MY PORT NUMBER IS [" << this->portNum_ << "]" << std::endl;
	}

};


class Socket
{
public:
	typedef std::vector<Block> vecBloc;

private:
	vecBloc vecBloc_;
	int		error_;

public:
	Socket(void);
	Socket(std::vector<Block> block);
	Socket(const Socket &other);
	~Socket(void);

	int		create_socket(); // return socket fd
	int		&socket_nonBlock_setting(int &socketFd);
	int		&socket_reUse_setting(int &socketFd);

	// Block class or utile
	vecBloc	getter_vecBloc() const;
};