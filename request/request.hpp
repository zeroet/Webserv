<<<<<<< HEAD
// #pragma once

// #include <iostream>
// #include <cstdio>
// #include <sys/epoll.h>
// #include <map>
// #include <vector>
// #include <algorithm>
// #include <cctype>

// #include "../server/connection.hpp"
// #include "../server/epoll.hpp"

// // # path: /return_test/index.html
// // # filepath = /var/bbb/aaa/index.html

// namespace ft {

// typedef struct s_uri {
//   std::string schema_;
//   std::string host_;
//   std::string port_;
//   std::string path_;
//   std::string filepath_;
//   std::string query_string_;
// } t_uri;

// typedef std::map<std::string, std::string> 	mapHeader;
// /*
//  * Class to stock every filterd elements of http request message
//  */
// class Request {
// 	private:
// 		std::string 	method_;
// 		std::string 	uri_;
// 		t_uri			uri_struct_;
// 		std::string		version_;
// 		mapHeader		requestHeaders_;
// 		std::string		body_;

// 	public:
// 		Request(void);
// 		// Request(int fd);
// 		Request(Request const &x);
// 		const Request &operator=(const Request &x);
// 		~Request(void);

// 		//getter
// 		const	std::string	&getMethod(void) const;
// 		const	std::string &getUri(void)	const;
// 		const	std::string	&getVersion(void)	const;
// 		const	mapHeader	&getRequestHeaders(void)	const;		//Get all headers in the shape of Map Container
// 		const	std::string &getHeaderValue(const std::string &key);	//Get one of header value
// 		const	std::string	&getBody(void)	const;
// 		const 	std::string &getSchema(void) const;
// 		const 	std::string &getHost(void) const;
// 		const 	std::string &getPort(void) const;
// 		const 	std::string &getPath(void) const;
// 		const 	std::string &getFilePath(void) const;
// 		const 	std::string &getQueryString(void) const;

// 		//setter
// 		void	setMethod(std::string method);
// 		void	setUri(std::string path);
// 		void	setVersion(std::string version);
// 		void	setHeader(std::string key, std::string value);		//Set one of header value
// 		void	setBody(std::string body);
// 		void	setSchema(std::string schema);
// 		void	setHost(std::string host);
// 		void 	setPort(std::string port);
// 		void 	setPath(std::string path);
// 		void 	setFilePath(std::string filepath);
// 		void 	setQueryString(std::string query_string);

// 		void	clear(void);

// 		//tmp
// 		void	printHeaders(void);
// };

// }
=======
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

// # path: /return_test/index.html
// # filepath = /var/bbb/aaa/index.html

namespace ft {

typedef struct s_uri {
  std::string schema_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::string filepath_;
  std::string query_string_;
} t_uri;

typedef std::map<std::string, std::string> 	mapHeader;
/*
 * Class to stock every filterd elements of http request message
 */
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

}
>>>>>>> ee1e01f7b3565ceaff46738994a699397c5ac1e2
