#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include <sstream>
#include "../request/request.hpp"
#include "mimeType.hpp"

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

	private:
		ft::mapHeader 							headers_;
		ft::mapHeader 							mapStatus_;

	public:
		Response();
		Response(Response const & copy);
		Response &operator=(Response const & copy);
		~Response();

		/* ************************************ */
		/* ************** setter ************** */
		/* ************************************ */
		void						setRequestValue(ft::Request &request);


		/* ************************************ */
		/* ************** getter ************** */
		/* ************************************ */
		std::string					makeErrorPage(int status_code);

	private:
		
		void						initialMapStatusCode(void);
		void						initialMapHeaders(void);
		
		/* ************************************ */
		/* ************** utils *************** */
		/* ************************************ */
		std::string			getExt(std::string const &filename) const;
		void				setValueFromRequest(ft::Request const &request);
		void				setContentType(ft::Request const &request);
		std::string 		toString(const int& v);
		
		/* ************************************ */
		/* ************** tester ************** */
		/* ************************************ */
		void				printMapHeader(ft::mapHeader	mapHeader) const;
	};

#endif
