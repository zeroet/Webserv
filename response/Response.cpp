#include "Response.hpp"

void		Response::initialMapHeaders(void)
{
  this->headers_["Allow"] = "";
  this->headers_["Content-Location"] = "";
  this->headers_["Last-Modified"] = "";
  this->headers_["Location"] = "";
  this->headers_["Retry-After"] = "";
  this->headers_["Server"] = "";
  this->headers_["Transfer-Encoding"] = "";
  this->headers_["WWW-Authenticate"] = "";
  this->headers_["Content-Language"] = "";
  this->headers_["Content-Length"] = "";
  this->headers_["Content-Type"] = "";
}

void		Response::initialMapStatusCode()
{
	mapStatus_.insert(std::make_pair("100", "Continue"));
	mapStatus_.insert(std::make_pair("101", "Switching Protocols"));
	mapStatus_.insert(std::make_pair("103", "Early Hints"));
	mapStatus_.insert(std::make_pair("200", "OK"));
	mapStatus_.insert(std::make_pair("201", "Created"));
	mapStatus_.insert(std::make_pair("202", "Accepted"));
	mapStatus_.insert(std::make_pair("203", "Non-Authoritative Information"));
	mapStatus_.insert(std::make_pair("204", "No Content"));
	mapStatus_.insert(std::make_pair("205", "Reset Content"));
	mapStatus_.insert(std::make_pair("206", "Partial Content"));
	mapStatus_.insert(std::make_pair("300", "Multiple Choices"));
	mapStatus_.insert(std::make_pair("301", "Moved Permanently"));
	mapStatus_.insert(std::make_pair("302", "Found"));
	mapStatus_.insert(std::make_pair("303", "See Other"));
	mapStatus_.insert(std::make_pair("304", "Not Modified"));
	mapStatus_.insert(std::make_pair("305", "Use Proxy"));
	mapStatus_.insert(std::make_pair("306", "unsued"));
	mapStatus_.insert(std::make_pair("307", "Temporary Redirect"));
	mapStatus_.insert(std::make_pair("308", "Permanent Redirect"));
	mapStatus_.insert(std::make_pair("400", "Bad Request"));
	mapStatus_.insert(std::make_pair("401", "Unauthorized"));
	mapStatus_.insert(std::make_pair("402", "Payment Required"));
	mapStatus_.insert(std::make_pair("403", "Forbidden"));
	mapStatus_.insert(std::make_pair("404", "Not Found"));
	mapStatus_.insert(std::make_pair("405", "Method Not Allowed"));	
	mapStatus_.insert(std::make_pair("408", "Request Timeout"));	
	mapStatus_.insert(std::make_pair("411", "Length Required"));	
	mapStatus_.insert(std::make_pair("413", "Payload Too Large"));	
	mapStatus_.insert(std::make_pair("414", "URI Too Long"));	
	mapStatus_.insert(std::make_pair("500", "Internal Server Error"));	
	mapStatus_.insert(std::make_pair("502", "Bad Gateway"));	
	mapStatus_.insert(std::make_pair("505", "HTTP Version Not Supported"));	
}
		

Response::Response()
{
	initialMapStatusCode();
	initialMapHeaders();
}

Response::Response(Response const & copy)
{
	(*this) = copy;
}

Response & Response::operator=(Response const & copy)
{
	(void) copy;
	return (*this);
}

Response::~Response()
{
}

std::string	Response::getExt(std::string const &filename) const
{
    char *buf = const_cast<char*>(filename.c_str());
   // bool ret = false;
    char* ptr = NULL;
 
    ptr = strrchr(buf, '.');
    if (ptr == NULL)
        return NULL;
 
    strcpy(buf, ptr + 1);
	std::string ret(buf);
    return ret;
}
