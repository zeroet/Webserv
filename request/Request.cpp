#include "Request.hpp"

Request::Request(void) {}

Request::~Request(void) {}

std::string	const	&Request::getMethod(void) const {
	return (this->Method_);
}

std::string	const	&Request::getPath(void) const {
	return (this->Path_);
}

std::string	const	&Request::getVersion(void) const {
	return (this->Version_);
}

std::map<std::string, std::string>	const	&Request::getRequestHeader(void) const {
	return (this->RequestHeader_);
}

std::string	const	&Request::getRequestBody(void) const {
	return (this->RequestBody_);
}