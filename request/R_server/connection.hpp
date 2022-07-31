#pragma once

#include "epoll.hpp"
#include "request.hpp"
#include "socket.hpp"
#include <iostream>
#include <cstdio>
#include <map>
#include <vector>

# define CRLF "\r\n"
# define END_BUF "\r\n\r\n"
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
		Request     request_;
		unsigned char               buffer[BUFFER_SIZE];
        std::vector<unsigned char>  buf_;
		// res
		// exe
	public:
		Connection(int fd, Block block, Epoll *ep);
		~Connection();

		void    requestRecv();
		void    response();

};