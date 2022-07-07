// Socket class 
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <vector>

// socket && epoll header
#include <sys/unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LISTEN_BACKLOG 15
#define ERROR 1
#define OK 0

class Block
{
private:
	int fd_;

public:
	Block() {};
	int getter() { return this->fd_; };
};


class Socket 
{
public:
	Socket(void);
	Socket(const std::vector<Block> &block);
	Socket(const Socket &other);
	~Socket(void);

	int		create_socket(); // return socket fd
	int		serverSocket_setting(int socketFd);

	vecBloc	getter_vecBloc() const;


private:
	vecBloc vecBloc_;
	int		error_;

};
