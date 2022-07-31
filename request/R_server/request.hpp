#pragma once

#include <iostream>
#include <cstdio>
#include "socket.hpp"
// #include "connection.hpp"
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
// #include <cstdlib>

typedef std::map<std::string, std::string> 	mapHeader;
typedef std::vector<unsigned int>			vecByte;

namespace ft {
	enum phaseMsgStat {
		START_LINE_INCOMPLETE,
		START_LINE_COMPLETE,
		HEADER_LINE_INCOMPLETE,
		HEADER_LINE_COMPLETE,
		BODY_INCOMPLETE,
		BODY_COMPLETE,
		ERROR_OCR
	};
}

class Request
{
	private:
		std::string 	method_;
		std::string 	path_;
		std::string		version_;
		mapHeader		requestHeaders_;
		vecByte			body_;
		ft::phaseMsgStat 	phase_;
		
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
		const	mapHeader	&getRequestHeaders(void)	const;
		const	std::string	getHeaderValue(std::string key) const;
		const	vecByte		&getBody(void)	const;

		//setter
		void	setPhaseMsgStat(ft::phaseMsgStat msg);

		//tmp
		void	printRequestMsg(void);

		// void    addBuffer(void);
		// void    parsingBuffer(void);
		//getter
		// std::string const &getBuffer(void) const;
		// std::vector<unsigned char> &getBuffer(void);
};