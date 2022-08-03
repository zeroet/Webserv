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

#include "../../config/src/ServerBlock.hpp"

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

class HttpBlcok;
class ServerBlock;
class BaseDirectives;

class Socket
{
public:
	typedef std::vector<ServerBlock> vecBloc;

private:
	vecBloc vecBloc_;
	int		error_;

public:
	Socket(void);
	Socket(std::vector<ServerBlock> block);
	Socket(const Socket &other);
	~Socket(void);

	int		create_socket(); // return socket fd
	int		&socket_nonBlock_setting(int &socketFd);
	int		&socket_reUse_setting(int &socketFd);

	// Block class or utile
	vecBloc	getter_vecBloc() const;
};
