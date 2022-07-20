#include "Response.hpp"

void		Response::map_make_pair(void)
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

	std::map<std::string, std::string>::iterator it;
	it = this->mapStatus_.find("100");
	this->StatusCode_ = it->first;
	this->StatusMessage_ = it->second;	
	std::cout << StatusCode_ << std::endl;
	std::cout << StatusMessage_ << std::endl;
}
		

Response::Response()
{
}

Response::Response(Response const & copy)
{
	(*this) = copy;
}

Response::Response(const std::string &status)
{
	std::cout << this->StatusCode_ << std::endl;
	std::cout << this->StatusMessage_ << std::endl;
	map_make_pair();
	response_manager();
}

Response & Response::operator=(Response const & copy)
{
	(void) copy;
	return (*this);
}

Response::~Response()
{
}

 void	Response::response_manager()
 {
 	std::string str;

 	str = makeHeader();
 	std::cout << str << std::endl;
 	makeStatusBody();
 	std::cout << Body_ << std::endl;
 }

void Response::appendHeader(std::string first, std::string second)
{
	Headers_.insert(std::make_pair(first, second));
}

void Response::setBody(std::string & str)
{
	this->Body_ = str;
}

std::string Response::makeHeader()
{
	std::string outcome;
	
	outcome.append("HTTP/1.1 " + this->StatusCode_ + " " + this->StatusMessage_ + "\r\n");
	for (std::map<std::string, std::string>::iterator it = Headers_.begin(); it != Headers_.end(); it++)
		outcome.append((*it).first + ": " + (*it).second + "\r\n");
	outcome.append("\r\n");
	return outcome;
}

void Response::makeStatusBody()
{
	std::string outcome;
	
	outcome.append("<!DOCTYPEhtml><html><head><meta charset=\"UTF-8\"/><title>webserv</title></head>");
	outcome.append("<Body_>");
	outcome.append("<h1>" + StatusCode_ + "</h1>");
	outcome.append("<h3>" + StatusMessage_ + "/h3>");
	outcome.append("<p>Click <a href=\"/\">here</a> to return home.</p>");
	outcome.append("</Body_></html>\r\n\r\n");
	
	Body_.clear();
	Body_ = outcome;
}

void Response::makeStatusBody(std::string url)
{
	std::string outcome;
	outcome.append(url);
	Body_.clear();
	Body_ = outcome;
}

