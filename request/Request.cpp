#include "Request.hpp"

Request::Request(void) {}

Request::~Request(void) {}

// void				Request::setRequest(Request *req) {
// }

void	Request::setMethod(std::string method) {
	this->Method_ = method;
}

void	Request::setPath(std::string path) {
	this->Path_ = path;
}

void	Request::setVersion(std::string version) {
	this->Version_ = version;
}

void	Request::setRequestBody(std::string body) {
	this->RequestBody_ = body;
}

std::string	const	&Request::getMethod(void) const {
	return (this->Method_);
}

std::string	const	&Request::getPath(void) const {
	return (this->Path_);
}

std::string	const	&Request::getVersion(void) const {
	return (this->Version_);
}

std::map<std::string, std::string>	&Request::getRequestHeader(void) {
	return (this->RequestHeader_);
}

std::string	const	&Request::getRequestBody(void) const {
	return (this->RequestBody_);
}

void		Request::GET1Req(Request *req) {
	req->setMethod("GET");
	req->setPath("/");
	req->setVersion("HTTP/1.1");
	req->RequestHeader_["Host"] = "127.0.0.1";
}

void		Request::GET2Req(Request *req) {
	req->setMethod("GET");
	req->setPath("/");
	req->setVersion("HTTP/1.1");
	req->RequestHeader_["Authorization"] = "Basic dXNlcjI6a2ltY2hpCgo=";
}

void		Request::GET3Req(Request *req) {
	req->setMethod("GET");
	req->setPath("/");
	req->setVersion("HTTP/1.1");
	req->RequestHeader_["Authorization"] = "Basic dXNlcjI6a2ltY2hpCgo=";
}

void		Request::POST1Req(Request *req) {
	req->setMethod("POST");
	req->setPath("/cgi-bin/process.cgi");
	req->setVersion("HTTP/1.1");
	req->RequestHeader_["User-Agent"] = "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)";
	req->RequestHeader_["HOST"] = "127.0.0.1";
	req->RequestHeader_["Content-Type"] = "text/xml; charset=utf-8";
	req->RequestHeader_["Content-Length"] = "length";
	req->RequestHeader_["Accept-Language"] = "en-us";
	req->RequestHeader_["Accept-Encoding"] = "gzip, deflate";
	req->RequestHeader_["Connection"] = "Keep-Alive";
	req->setRequestBody("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<string xmlns=\"http://clearforest.com/\">string</string>");
}

void		Request::POST2Req(Request *req) {
	req->setMethod("POST");
	req->setPath("/testing.php");
	req->setVersion("HTTP/1.1");
	req->RequestHeader_["HOST"] = "127.0.0.1";
	req->setRequestBody("<!DOCTYPE html>\n<html>\n<body>\n<?php\n$color = \"red\";\necho \"My car is \" . $color . \"<br>\";\necho \"My house is \" . $COLOR . \"<br>\";\necho \"My boat is \" . $coLOR . \"<br>\";\n?>\n</body>\n</html>");
}

void	Request::PUTReq(Request *req) {
	req->setMethod("PUT");
	req->setPath("/new.html");
	req->setVersion("HTTP/1.1");
	req->RequestHeader_["Host"] = "127.0.0.1";
	req->RequestHeader_["Content-type"] = "text/html";
	req->RequestHeader_["Content-length"] = "16";
	req->setRequestBody("<p>New File</p>");
}

void	Request::DELETEReq(Request *req) {
	req->setMethod("DELETE");
	req->setPath("/abcd.html");
	req->setVersion("HTTP/1.1");
	req->RequestHeader_["Host"] = "127.0.0.1";
}