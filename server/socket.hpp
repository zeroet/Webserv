

#include <iostream>
#include <string.h>

// socket && epoll header
#include <sys/unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LISTEN_BACKLOG 15

class Socket 
{
public:
	Socket(void);
	Socket(const Socket &other);
	~Socket(void);

	int		create_socket(); // return socket fd
	int		init_bind_socket(); // if  return < 0  = error
	void	litsen_socket(int server_fd, int listen_backlog); // listen q

	void	setSocketFd(int fd);
	int		getSocketFd(void);

private:
	int		mSocketFd;
	int		mError;

};
