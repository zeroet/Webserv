#include "request.hpp"

Request::Request(void) : method_(""), path_(""), version_(""), requestHeaders_(), body_("") {}

Request::Request(Request const &x) {
	*this = x;
}

const	Request	&Request::operator=(const Request &x) {
	if (this != &x) {
    method_ = x.method_;
    path_ = x.path_;
	version_ = x.version_;
    requestHeaders_ = x.requestHeaders_;
    body_ = x.body_;
  }
  return (*this);
}

Request::~Request(void) {}

//getter
const	std::string	&Request::getMethod(void) const {
	return (method_);
}

const	std::string &Request::getPath(void)	const {
	return (path_);
}

const	std::string	&Request::getVersion(void)	const {
	return (version_);
}

const	mapHeader	&Request::getRequestHeaders(void)	const {
	return (requestHeaders_);
}


const std::string &Request::getHeaderValue(const std::string &key) {
	return (requestHeaders_[key]);
}
// const	std::string	Request::getHeaderValue(std::string key) const {
// 	std::transform(key.begin(), key.end(), key.begin(), toupper);
// 	for (mapHeader::const_iterator it = requestHeaders_.begin(); it != requestHeaders_.end(); it++)
// 	{
// 		if (it->first == key)
// 			return (it->second);
// 	}
// 	return ("");
// }

const	std::string		&Request::getBody(void)	const {
	return (body_);
}

//setter
void	Request::setMethod(std::string method) {
	method_ = method;
}

void	Request::setPath(std::string path) {
	path_ = path;
}

void	Request::setVersion(std::string version) {
	version_ = version;
}

void	Request::setHeader(std::string key, std::string value) {
	requestHeaders_[key] = value;
}

void	Request::setBody(std::string body) {
	body_ = body;
}


//tmp
void	Request::printHeaders(void) {
	std::cout << "Request message headers:" << std::endl;
	for(mapHeader::iterator it = requestHeaders_.begin(); it != requestHeaders_.end(); it++)
		std::cout << (*it).first << ": " << (*it).second << std::endl;
}
