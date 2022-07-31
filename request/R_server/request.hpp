#pragma once

#include <iostream>
#include <cstdio>
#include "socket.hpp"
// #include "connection.hpp"
#include <sys/epoll.h>
#include <map>
#include <vector>

typedef std::map<std::string, std::string> 	mapHeader;
typedef std::vector<unsigned int>			vecByte;

class Request
{
	private:
		std::string method_;
		std::string path_;
		std::string	version_;
		mapHeader	requestHeaders_;
		vecByte		body_;
		

	public:
		Request(void);
		// Request(int fd);
		Request(Request const &x);
		const Request &operator=(const Request &x);
		~Request(void);

		// void    addBuffer(void);
		// void    parsingBuffer(void);
		//getter
		// std::string const &getBuffer(void) const;
		// std::vector<unsigned char> &getBuffer(void);
};