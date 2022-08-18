#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
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

namespace ft 
{

class MimeType;
class Request;

class Response
{

private:
	mapHeader 								headers_;
	mapHeader 								mapStatus_;
	MimeType								mimeType_;
	Request									request_;

public:
	Response();
	Response(Response const & copy);
	Response &operator=(Response const & copy);
	~Response();
	
	/* ************************************ */
	/* ************** setter ************** */
	/* ************************************ */
	void						setRequest(Request const &request);
	
	/* ************************************ */
	/* ************** getter ************** */
	/* ************************************ */
	
	std::string					makeBodyPage(Request const &Request);
	std::string					makeErrorPage(int status_code);
	std::string					makeBodyHtml(std::string const &filePath);				
	std::string					makeHeader(int bodySize);
	std::string					getExt(std::string const &filename) const;
private:
	
	/* ************************************ */
	/* ************** utils *************** */
	/* ************************************ */
	void						setRequestValue(void);
	void						initialMapHeaders(void);
	void						initialMapStatusCode(void);
	void						setValueFromRequest(void);
	void						setContentType(void);
	std::string 				toString(const int& v);
		
	/* ************************************ */
	/* ************** tester ************** */
	/* ************************************ */
	void						printMapHeader(ft::mapHeader	mapHeader) const;
};

}
#endif
