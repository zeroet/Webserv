// Socket class 
// 


#include <iostream>
#include <string.h>
#include <fcntl.h>

// socket && epoll header
#include <sys/unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LISTEN_BACKLOG 15
#define ERROR 1
#define OK 0

class Config
{
private:
	int fd_;

public:
	Config(); {}
	int getter() { return this->fd_; };
};


class Socket 
{
public:
	Socket(void);
	Socket(const std::vecotr<Block> block);
	Socket(const Socket &other);
	~Socket(void);

	int		create_socket(); // return socket fd


	// void	setSocketFd(int fd);
	// int		getSocketFd(void);

private:
	typedef std::vector<int>	vecServ_;
	int		error_;

};
