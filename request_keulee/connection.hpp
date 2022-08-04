#pragma once

#include "epoll.hpp"
#include "request.hpp"
#include "../response/Response.hpp"
#include "socket.hpp"
#include "OperateRequest.hpp"
#include "../config/src/ServerBlock.hpp"

#include <iostream>
#include <cstdio>
#include <map>
#include <vector>

# define CRLF "\r\n"
# define CRLFCRLF "\r\n\r\n"
# define LEN_CRLF 2
# define LEN_CRLFCRLF 4

# define BUFFER_SIZE 512

//class Request;
class Epoll;
class Request;
class OperateRequest;


enum phaseMsg {
	START_LINE_INCOMPLETE,
	START_LINE_COMPLETE,
	HEADER_INCOMPLETE,
	HEADER_COMPLETE,
	BODY_INCOMPLETE,
	BODY_COMPLETE,
	BODY_CHUNKED,
	ERROR_OCR
};

enum RequestStatusCode {
	NOT_DEFINE = -1,
	ALL_OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NO_CONTENT = 204,
	MOVED_PERMANENTLY = 301,
	TEMPORARY_REDIRECT = 307,
	PERMANENT_REDIRECT = 308,
	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	REQUEST_TIMEOUT = 408,
	PAYLOAD_TOO_LARGE = 413,
	SERVER_ERROR = 500,
	BAD_GATEWAY = 502,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

class Connection
{
	private:
		int         clntFd_;
		ServerBlock   block_;
		std::string status_;
		Epoll       *ep_; //Epoll stokage

		char			buffer_char[BUFFER_SIZE]; 	//get char from recv
		std::string		buffer_;					//append buffer

		Request     			request_;
		// std::vector<Request>	multi_request;
		Response		response_;

		int				phase_msg_;
		int				req_status_code_;
		// res
		// exe

	public:
		Connection(int fd, ServerBlock block, Epoll *ep);
		~Connection();

		//getter
		ServerBlock	&getBlock(void);
		Request		&getRequest(void);
		Response	&getResponse(void);
		int			&getPhaseMsg(void);
		std::string &getBuffer(void);
		int			&getReqStatusCode(void);

		//setter
		void	setPhaseMsg(int new_msg);
		void	setReqStatusCode(int status_code);

		void    processRequest();
		void    response();

		//tmp
		void	printRequestMsg(void);

};
