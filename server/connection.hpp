#pragma once

#include "epoll.hpp"
#include "socket.hpp"
#include "../request/request.hpp"
#include "../request/RequestHandler.hpp"
#include "../response/Response.hpp"
#include "../response/cgi.hpp"
#include "../config/src/ServerBlock.hpp"


#include <iostream>
#include <cstdio>
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>

# define CRLF "\r\n"
# define CRLFCRLF "\r\n\r\n"
# define LEN_CRLF 2
# define LEN_CRLFCRLF 4

# define BUFFER_SIZE 512

namespace ft{

class Epoll;

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
	CONFLICT = 409,
	PAYLOAD_TOO_LARGE = 413,
	SERVER_ERROR = 500,
	BAD_GATEWAY = 502,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

enum ChunkedMessageStatus {
  STR_SIZE = 0,
  STR,
  END
};

class Connection
{
	private:
		int							clntFd_;
		std::vector<ServerBlock>  	block_;
		std::string 				status_;
		Epoll						*ep_;
		ServerBlock					serverConfig_;
		LocationBlock				locationConfig_;

		Request		request_;
		Response	response_;

		int				phase_msg_;
		int				req_status_code_;

	public:

		char			buffer_char[BUFFER_SIZE]; 	//get char from recv
		std::string		buffer_;					//append buffer
		size_t			client_max_body_size;
		int				buffer_content_length;
		int				is_chunk;
		int				chunked_msg_checker;
		size_t			chunked_msg_size;
		std::string		body_buf;
		// res
		// exe

	public:
		Connection(int fd, std::vector<ServerBlock> block, Epoll *ep);
		~Connection();
		//getter
		std::vector<ServerBlock>	&getBlock(void);
		Request						&getRequest(void);
		ft::Response					&getResponse(void);
		int							&getPhaseMsg(void);
		std::string 				&getBuffer(void);
		int							&getReqStatusCode(void);
		std::string					&getBodyBuf(void);
		ServerBlock					getServerConfig(void);
		LocationBlock				getLocationConfig(void);
		std::string					getStatus(void);

		//setter
		void		setPhaseMsg(int new_msg);
		void		setReqStatusCode(int status_code);
		void		setServerBlockConfig(std::string server_name);
		bool		checkLocationConfigExist(std::string path);
		void		setLocationConfig(LocationBlock locationblock);
		void		setBodyBuf(std::string bodybuf);

		//utils
		ServerBlock 	getServerConfigByServerName(std::string server_name);
    
		void    		processRequest(void);
		void    		processResponse(void);
		
		//tmp
		void	printRequestMsg(void);
};

}
