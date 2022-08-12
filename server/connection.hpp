#pragma once

#include "epoll.hpp"
#include "socket.hpp"
#include "../request/request.hpp"
#include "../request/OperateRequest.hpp"
#include "../response/Response.hpp"
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

typedef struct s_uri {
  std::string schema_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::string filepath_;
  std::string query_string_;
} t_uri;

typedef std::map<std::string, std::string> 	mapHeader;
class Request {	
	private:
		std::string 	method_;
		std::string 	uri_;
		t_uri			uri_struct_;
		std::string		version_;
		mapHeader		requestHeaders_;
		std::string		body_;

	public:
		Request(void);
		// Request(int fd);
		Request(Request const &x);
		const Request &operator=(const Request &x);
		~Request(void);

		//getter
		const	std::string	&getMethod(void) const;
		const	std::string &getUri(void)	const;
		const	std::string	&getVersion(void)	const;
		const	mapHeader	&getRequestHeaders(void)	const;		//Get all headers in the shape of Map Container
		const	std::string &getHeaderValue(const std::string &key);	//Get one of header value
		const	std::string	&getBody(void)	const;
		const 	std::string &getSchema(void) const;
		const 	std::string &getHost(void) const;
		const 	std::string &getPort(void) const;
		const 	std::string &getPath(void) const;
		const 	std::string &getFilePath(void) const;
		const 	std::string &getQueryString(void) const;

		//setter
		void	setMethod(std::string method);
		void	setUri(std::string path);
		void	setVersion(std::string version);
		void	setHeader(std::string key, std::string value);		//Set one of header value
		void	setBody(std::string body);
		void	setSchema(std::string schema);
		void	setHost(std::string host);
		void 	setPort(std::string port);
		void 	setPath(std::string path);
		void 	setFilePath(std::string filepath);
		void 	setQueryString(std::string query_string);

		void	clear(void);

		//tmp
		void	printHeaders(void);
};


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
		int							clntFd_;
		std::vector<ServerBlock>  	block_;
		std::string 				status_;
		Epoll						*ep_;
		ServerBlock					serverConfig_;
		LocationBlock				locationConfig_;

		char			buffer_char[BUFFER_SIZE]; 	//get char from recv
		std::string		buffer_;					//append buffer

		// Request			request_;
		// Response		response_;
		Request		request_;
		Response	response_;

		int				phase_msg_;
		int				req_status_code_;

	public:
		size_t			client_max_body_size;
		size_t			content_length;
		int				is_chunk;
		std::string		body_buf;
		// res
		// exe

	public:
		Connection(int fd, std::vector<ServerBlock> block, Epoll *ep);
		~Connection();
		//getter
		std::vector<ServerBlock>	&getBlock(void);
		Request						&getRequest(void);
		Response					&getResponse(void);
		int							&getPhaseMsg(void);
		std::string 				&getBuffer(void);
		int							&getReqStatusCode(void);
		ServerBlock					getServerConfig(void);
		LocationBlock				getLocationConfig(void);

		//setter
		void		setPhaseMsg(int new_msg);
		void		setReqStatusCode(int status_code);
		void		setServerBlockConfig(std::string server_name);
		bool		checkLocationConfigExist(std::string path);
		void		setLocationConfig(LocationBlock locationblock);

		//utils
		ServerBlock 	getServerConfigByServerName(std::string server_name);
		// LocationBlock	*getLocationConfigByPath(std::string path);
		void    		processRequest(void);
		void    		response(void);

		//tmp
		void	printRequestMsg(void);
};

}
