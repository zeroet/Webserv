#include "OperateRequest.hpp"

OperateRequest::OperateRequest(void) {}

OperateRequest::~OperateRequest(void) {}

//getter
std::string	&OperateRequest::getStartLine(void) {
	return (startLine_);
}
std::string	&OperateRequest::getHeaders(void) {
	return (headers_);
}

//setter

void	OperateRequest::checkRequestMessage(Connection *c) {

	size_t pos = 0;
	if (c->getPhaseMsg() == START_LINE_INCOMPLETE)
	{
		if ((pos = c->getBuffer().find(CRLF))!= std::string::npos)
			c->setPhaseMsg(START_LINE_COMPLETE);
	}
	if (c->getPhaseMsg() == START_LINE_COMPLETE)
	{
		//parse start line
		std::cout << "Parse Start Line" << std::endl;
		startLine_ = c->getBuffer().substr(0, pos);
		tmp_ = pos + LEN_CRLF;
		parseStartLine(c);
		// if ((pos = c->getBuffer().find(CRLFCRLF)) == std::string::npos)
		// 	c->setPhaseMsg(HEADER_INCOMPLETE);
	}
	if (c->getPhaseMsg() == HEADER_INCOMPLETE)
	{
		if ((pos = c->getBuffer().find(CRLFCRLF)) != std::string::npos)
			c->setPhaseMsg(HEADER_COMPLETE);
	}
	if (c->getPhaseMsg() == HEADER_COMPLETE)
	{
		std::cout << "Parse Header" << std::endl;
		// headers_ = c->getBuffer().substr(tmp_, c->getBuffer().length());
		headers_ = c->getBuffer().substr(tmp_, pos + LEN_CRLFCRLF);
		tmp_ = pos + LEN_CRLFCRLF;
		// std::cout << "header: " << headers_ << std::endl;
		// std::cout << "tmp_: " << tmp_ << std::endl;
		//parse header
		parseHeaders(c);
	}
}

void	OperateRequest::parseStartLine(Connection *c) {

	//method check : GET/POST/DELETE -> toupper / if not Error 400
	std::vector<std::string> split_start_line = splitDelim(startLine_, " ");
	if (split_start_line.size() != 3)
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(HEADER_COMPLETE);
		std::cout << "start line argument wrong request code : " <<  c->getReqStatusCode() <<  std::endl;
		return ;
	}
	// for (size_t i = 0; i < split_start_line.size(); i++)
	// 	std::cout << split_start_line[i] << std::endl;
	// std::string method = startLine_.substr(0, startLine_.find_first_of(" "));
	// std::cout << method << std::endl;
	if (!checkMethod(split_start_line[0]))
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(HEADER_COMPLETE);
		return ;
	}
	else
		c->getRequest().setMethod(split_start_line[0]);

	//path check:
	c->getRequest().setPath(split_start_line[1]);

	//HTTP/1.1 check: 'HTTP/' '1.*' / if not Error 400/505
	std::string http = split_start_line[2].substr(0, 5);
	std::string version = split_start_line[2].substr(5, split_start_line[2].length());
	int cmp = http.compare("HTTP/");
	if (!checkVersion(version) || version.length() != 3 || cmp)
	{
		std::cout << "HERE" << std::endl;
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(HEADER_COMPLETE);
		return ;
	}
	cmp = version.compare(0, 2, "1.");
	if (cmp)
	{
		c->setReqStatusCode(HTTP_VERSION_NOT_SUPPORTED);
		c->setPhaseMsg(HEADER_COMPLETE);
		return ;
	}
	c->getRequest().setVersion(http + version);
	c->setPhaseMsg(HEADER_INCOMPLETE);
}

void	OperateRequest::parseHeaders(Connection *c) {
	(void)c;
	std::cout << "headers" << std::endl;
	std::cout << headers_ << std::endl;
	size_t pos = 0;
	int code = 0;
	while ((pos = headers_.find(CRLF)) != std::string::npos)
	{
		std::string headerline = "";
		// std::cout << "pos: " << pos << std::endl;
		headerline = headers_.substr(0, pos);
		// std::cout << "Header line " << i++ << " : " << headerline << std::endl;
		// }

		//headerline parse - argument is < 2 => 400 // if field name is not alpha => 400
													// if there's sth between fild name and colon => 400
		if ((code = parseHeaderLine(c, headerline)) != DEFAULT)
		{
			c->setReqStatusCode(code);
			return ;
		}
		headers_ = headers_.substr(pos + LEN_CRLF, headers_.length());
		// std::cout << "headers to parse line : " << std::endl;
		std::cout << headers_ << std::endl;
		if (headers_.length() == LEN_CRLF)
			break ;
	}
	// std::cout <<
}

int		OperateRequest::parseHeaderLine(Connection *c, std::string headerline) {
	std::vector<std::string> header_line_parse = splitDelim(headerline, ":");
	if (!checkHeaderValue(header_line_parse[0]) || header_line_parse.size() != 2)
		return (BAD_REQUEST);
	std::string str = trimWhiteSpace(header_line_parse[1]);

	std::string key = header_line_parse[0];
	std::string value = str;
	c->getRequest().setHeader(key, value);
	return (DEFAULT);
}

//utiles
std::vector<std::string> OperateRequest::splitDelim(std::string s, std::string delim) {
    size_t pos_start = 0, pos_end, delim_len = delim.length();
    std::string 		token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delim, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

int		OperateRequest::checkMethod(const std::string &s) {

	for(size_t i = 0; i < s.length(); i++)
	{
		char c = s[i];
		if (!isupper(c) || !isalpha(c))
			return (false);
	}
	return (true);
}

int			OperateRequest::checkVersion(const std::string &s) {
	for (size_t i = 0; i < s.length(); i++)
	{
		char c = s[i];
		if (isdigit(c) || (c == '.'))
			i++;
		else
			return (false);
	}
	return (true);
}

int			OperateRequest::checkHeaderValue(const std::string &s) {
	for (size_t i = 0; i < s.length(); i++)
	{
		char c = s[i];
		if (!isalpha(c))
			return (false);
	}
	return (true);
}

std::string	OperateRequest::trimWhiteSpace(std::string &s) {

	std::string str;
	std::string whitespace(" \n\r\t\f\v");
	size_t found1 = s.find_first_not_of(whitespace);
	size_t found2 = s.find_last_not_of(whitespace);
	// std::cout << "string length: " << s.length() << std::endl;
	// std::cout << "find first not of whitespace : " << found1 << std::endl;
	// std::cout << "find last not of whitespace : " << found2 << std::endl;
	str = s.substr(found1, found2 - found1 + 1);
	// std::cout << "length after this: " << str.length() << std::endl;
	return (str);
}
