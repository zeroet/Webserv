#pragma once

#include <iostream>
#include <cstdio>
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>

#include "../server/connection.hpp"
#include "../server/epoll.hpp"

namespace ft {

typedef std::map<std::string, std::string> 	mapHeader;
/*
 * Class to stock every filterd elements of http request message
 */
class Request {
	private:
		std::string 	method_;
		std::string 	path_;
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
		const	std::string &getPath(void)	const;
		const	std::string	&getVersion(void)	const;
		const	mapHeader	&getRequestHeaders(void)	const;		//Get all headers in the shape of Map Container
		const	std::string	getHeaderValue(std::string key) const;	//Get one of header value
		const	std::string	&getBody(void)	const;

		//setter
		void	setMethod(std::string method);
		void	setPath(std::string path);
		void	setVersion(std::string version);
		void	setHeader(std::string key, std::string value);		//Set one of header value

		//tmp
		void	printHeaders(void);
};

}
