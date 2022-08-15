#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>

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
		typedef std::map<std::string, std::string> mapStatus;

	private:
		std::map<std::string, std::string> 	headers_;
		std::string 						statusCode_;
		std::string 						statusMessage_;
		std::string 						body_;
		mapStatus 							mapStatus_;
	
	public:
		Response();
		Response(Response const & copy);
		Response &operator=(Response const & copy);
		~Response();

		std::string			getBodyStr(std::string const &filePath);

	private:

		//std::string 	makeHeader();
		//void			appendHeader(std::string first, std::string second);
		
		/*============================*/
		/* ========== body ===========*/
		/*============================*/
		//void			setBody(std::string & str);
		//void			makeStatusBody();
		//void			makeStatusBody(std::string url);
		//int				getBodySize() const;
		
		/*===================================*/
		/* ========== status code ========== */
		/*===================================*/
		void			initialMapStatusCode(void);
		void			initialMapHeaders(void);
		
		//std::string		getStatusCode() const;
		//void			setStatusCode(std::string code);

		//void			executeGet(void);
		//void			executePost(void);
		//void			executeDelete(void);
	};

#endif
