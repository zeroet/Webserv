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


enum phaseMsg {
	START_LINE_INCOMPLETE,
	START_LINE_COMPLETE,
	HEADER_INCOMPLETE,
	HEADER_COMPLETE,
	BODY_INCOMPLETE,
	BODY_COMPLETE,
	ERROR_OCR
};

class Connection
{
	private:
		int         clntFd_;
		Block       block_;
		std::string status_;
		Epoll       *ep_;

		char			buffer_char[BUFFER_SIZE];
		std::string		buffer_;

		Request     			request_;
		// std::vector<Request>	multi_request;
		// response		response_;

		bool			Ctl_mode_flag_;
		int				phase_msg_;
		// ft::
		// res
		// exe
	public:
		Connection(int fd, Block block, Epoll *ep);
		~Connection();

		//getter
		Request	&getRequest(void);
		bool	&getCtlMode(void);
		int		&getPhaseMsg(void);
		std::string &getBuffer(void);

		//setter
		void	setCtlMode(bool mode);
		void	setPhaseMsg(int new_msg);

		void    processRequest();
		void    response();

};
