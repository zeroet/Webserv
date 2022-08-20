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
#include <time.h>

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
	mapHeader 								mapStatus_;
	mapHeader 								headers_;
	Request									request_;
	MimeType								mimeType_;

public:
	/* ************************************ */
	/* **** constructor / destructor ****** */
	/* ************************************ */
	Response();
	Response(Response const & copy);
	Response &operator=(Response const & copy);
	~Response();
	
	/* ************************************ */
	/* ************** setter ************** */
	/* ************************************ */
	void						setRequest(Request const &request);
	void						setRequestValue(void);
	
	/* ************************************ */
	/* *************** header ************** */
	/* ************************************ */
	std::string					makeHeader(int bodySize, int statusCode);

	/* ************************************ */
	/* ************ error page ************ */
	/* ************************************ */
	std::string					makeErrorPage(int status_code);

	/* ************************************ */
	/* *********** method = GET *********** */
	/* ************************************ */
	std::string					makeBodyHtml(std::string const &filePath);				

	/* ************************************ */
	/* ********* method = DELETE ********** */
	/* ************************************ */
	int							execteDelete(void);

	/* ************************************ */
	/* *************** utils ************** */
	/* ************************************ */
	std::string					getExt(std::string const &filename) const;

private:
	/* ************************************ */
	/* ************** initial ************* */
	/* ************************************ */
	void						initialMapHeaders(void);
	void						initialMapStatusCode(void);
	void						setValueFromRequest(void);


	/* ************************************ */
	/* ************** utils *************** */
	/* ************************************ */
	std::string 				toString(const int& v) const;

	/* ************************************ */
	/* ************ make header *********** */
	/* ************************************ */
	void						setContentType(void);
	std::string					makeTimeLine(void) const;
	void						setContentLengh(int bodySize);
	std::string					makeStartLine(int statusCode);
	std::string					appendMapHeaders();


	/* ************************************ */
	/* ************** tester ************** */
	/* ************************************ */
	void						printMapHeader(ft::mapHeader	mapHeader) const;
};

} // namespace ft

#endif
