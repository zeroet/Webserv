#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <cstring>

/* Headers to manage
Accept-Charsets
Accept-Language
Allow
Authorization
Content-Language
Content-Length
Content-Location
Content-Type
Date
Host
Last-Modified
Location
Referer
Retry-After
Server
Transfer-Encoding
User-Agent
WWW-Authenticate */


class Response
{

	public:
		typedef std::map<std::string, std::string> map_;

	private:
		map_ 							headers_;
		map_ 							mapStatus_;
	
	public:
		Response();
		Response(Response const & copy);
		Response &operator=(Response const & copy);
		~Response();

	private:
		
		void						initialMapStatusCode(void);
		void						initialMapHeaders(void);
		
		std::string			getExt(std::string const &filename) const;
	};

#endif
