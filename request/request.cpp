#include "../server/connection.hpp"

Request::Request(void) : method_(""), uri_("/"), version_(""), requestHeaders_(), body_("") {
	uri_struct_.path_ = "/";
}

Request::Request(Request const &x) {
	*this = x;
}

const	Request	&Request::operator=(const Request &x) {
	if (this != &x) 
	{
		method_ = x.method_;
		uri_ = x.uri_;
		version_ = x.version_;
		requestHeaders_ = x.requestHeaders_;
		body_ = x.body_;

		uri_struct_.schema_ = x.uri_struct_.schema_;
		uri_struct_.host_ = x.uri_struct_.host_;
		uri_struct_.port_ = x.uri_struct_.port_;
		uri_struct_.path_ = x.uri_struct_.path_;
		uri_struct_.filepath_ = x.uri_struct_.filepath_;
		uri_struct_.query_string_ = x.uri_struct_.query_string_;

		// sorry!
		// setFilePath(x.uri_struct_.filepath_);
		// setQueryString(x.uri_struct_.query_string_);
		// setBody(x.body_);
		// setUri(x.uri_);
		// setPath(x.uri_struct_.path_);
	}
	return (*this);
}

Request::~Request(void) {
	this->clear();
}

void	Request::clear(void) {
  	method_.clear();
  	uri_.clear();
  	version_.clear();
	requestHeaders_.clear();
  	body_.clear();

	uri_struct_.schema_.clear();
	uri_struct_.host_.clear();
	uri_struct_.port_.clear();
	uri_struct_.path_ = "/";
	uri_struct_.filepath_.clear();
	uri_struct_.query_string_.clear();
}

//getter
const	std::string	&Request::getMethod(void) const {
	return (method_);
}

const	std::string &Request::getUri(void)	const {
	return (uri_);
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

const 	std::string &Request::getSchema(void) const {
	return (uri_struct_.schema_);
}

const 	std::string &Request::getHost(void) const {
	return (uri_struct_.host_);
}

const 	std::string &Request::getPort(void) const {
	return (uri_struct_.port_);
}

const 	std::string &Request::getPath(void) const {
	return (uri_struct_.path_);
}

const 	std::string &Request::getFilePath(void) const {
	return (uri_struct_.filepath_);
}

const 	std::string &Request::getQueryString(void) const {
	return (uri_struct_.query_string_);
}


//setter
void	Request::setMethod(std::string method) {
	method_ = method;
}

void	Request::setUri(std::string uri) {
	uri_ = uri;
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

void Request::setSchema(std::string schema) {
	uri_struct_.schema_ = schema;
}

void Request::setHost(std::string host) {
	uri_struct_.host_ = host;
}

void Request::setPort(std::string port) {
	uri_struct_.port_ = port;
}

void Request::setPath(std::string path) {
	uri_struct_.path_ = path;
}

void Request::setFilePath(std::string filepath) {
	uri_struct_.filepath_ = filepath;
}
void Request::setQueryString(std::string query_string) {
	uri_struct_.query_string_ = query_string;
}


//tmp
void	Request::printHeaders(void) {
	std::cout << "Request message headers:" << std::endl;
	for(mapHeader::iterator it = requestHeaders_.begin(); it != requestHeaders_.end(); it++)
		std::cout << (*it).first << ": " << (*it).second << std::endl;
}
