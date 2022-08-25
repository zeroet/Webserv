#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <cstring>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <bits/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include "mimeType.hpp"
#include "../request/request.hpp"
#include "../config/src/ServerBlock.hpp"
#include "../config/src/LocationBlock.hpp"

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

# define CGI	0
# define HTML	1

class LocationBlock;
class MimeType;
class Request;

class Response
{

private:
	mapHeader 					mapStatus_;
	mapHeader 					headers_;
	Request						request_;
	LocationBlock				location_;
	MimeType					mimeType_;

public:
	/* ************************************ */
	/* **** constructor / destructor ****** */
	/* ************************************ */
	Response();
	Response(Response const & copy);
	Response &operator=(Response const & x);
	~Response();
	
	/* ************************************ */
	/* ************** setter ************** */
	/* ************************************ */
	void						setLocation(LocationBlock const &locationBlock);
	void						setRequest(Request const &request);
	void						setRequestValue(void);
	
	/* ************************************ */
	/* *************** header ************** */
	/* ************************************ */
	std::string					makeHeader(int bodySize, int statusCode);
	std::string					makeHeaderCgi(std::string &body, int statusCode);

	/* ************************************ */
	/* ************ error page ************ */
	/* ************************************ */
	std::string					makeErrorPage(int status_code);

	/* ************************************ */
	/* *********** method = GET *********** */
	/* ************************************ */
	std::string					makeBodyHtml(std::string const &filePath);				

	/* ************************************ */
	/* ************ autoindex ************* */
	/* ************************************ */
	std::string					bodyWithAutoindexOn(const std::string &uri, const std::string &filepath);
	std::string					getFileDateTime(time_t sec);
	std::string					fileTextIntoBody(const std::string &filepath);

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
	std::string					makeTimeLine(int option) const;
	void						setContentLengh(int bodySize);
	std::string					makeStartLine(int statusCode);
	std::string					appendMapHeaders(int option, int statusCode);


	/* ************************************ */
	/* ************** tester ************** */
	/* ************************************ */
	void						printMapHeader(ft::mapHeader	mapHeader) const;
	
};

} // namespace ft

#endif
