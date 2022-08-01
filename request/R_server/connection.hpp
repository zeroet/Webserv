#pragma once

#include "epoll.hpp"
#include "request.hpp"
#include "socket.hpp"
#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <deque>

# define CRLF "\r\n"
# define CRLFCRLF "\r\n\r\n"
# define LEN_END_BUF 4
# define LEN_CRLF 2

# define BUFFER_SIZE 512

//class Request;
class Epoll;
class Request;

class Connection
{
	private:
		int         clntFd_;
		Block       block_;
		std::string status_;
		Epoll       *ep_;

		char		buffer_char[BUFFER_SIZE];
		std::string		buffer_;

		Request     	request_;

		bool			Ctl_mode_flag_;
		// res
		// exe
	public:
		Connection(int fd, Block block, Epoll *ep);
		~Connection();

		Request	&getRequest(void);
		bool	&getCtlMode(void);

		void	setCtlMode(bool mode);

		void    processRequest();
		void    response();

};
