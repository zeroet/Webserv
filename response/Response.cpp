#include "response.hpp"

Response::Response(std::string status)
{
	this->status_code = status.substr(0, 3);
	// get 3 string(number) in the status.
	this->status_message = status.substr(4);
	//from index 4 to end
	//should be added values in the status.(ex: 200 ok);
}

Response::~Response();
{
}

void Response::appendHeader(std::string first, std::string second)
{
	mHeaders.insert(std::make_pair(first, second));
{

void Response::setBody(std::string & str)
{
	this->body = str;
}

std::string Response::makeHeader()
{
	std::string outcome;
	
	outcome.append("HTTP/1.1 " + status_code + " " + status_message + "\r\n");
	for (std::map<std::string, std::string>::iterator it = mHeaders.begin(); it != mHeaders.end(); it++)
		outcome.append((*it).first + ": " + (*it).second + "\r\n");
	outcome.append("\r\n");
	return outcome;
}

void Response::makeStatusBody()
{
	std::string outcome;
	
	outcome.append("<!DOCTYPEhtml><html><head><meta charset=\"UTF-8\"/><title>webserv</title></head>");
	outcome.append("<body>");
	outcome.append("<h1>" + status_code + "</h1>");
	outcome.append("<h3>" + status_message + "/h3>");
	outcome.append("<p>Click <a href=\"/\">here</a> to return home.</p>");
	outcome.append("</body></html>");
	
	body.clear();
	body = outcome();
}

void Response::makeStatusBody(std::string url)
{
	std::string outcome;
	outcome.append(url);
	body.clear;
	body = outcome;
}

