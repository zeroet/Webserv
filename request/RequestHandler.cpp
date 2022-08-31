#include "RequestHandler.hpp"

RequestHandler::RequestHandler(void) {}

RequestHandler::~RequestHandler(void) {}

//getter
std::string	&RequestHandler::getStartLine(void) {
	return (startLine_);
}
std::string	&RequestHandler::getHeaders(void) {
	return (headers_);
}

//setter
void	RequestHandler::setBody(std::string bodybuf_) {
	body_ = bodybuf_;
}

void	RequestHandler::checkRequestMessage(Connection *c) {

	if (c->getPhaseMsg() == START_LINE_INCOMPLETE)
	{
		size_t pos;
		if ((pos = c->getBuffer().find(CRLF)) != std::string::npos)
			c->setPhaseMsg(START_LINE_COMPLETE);
	}
	if (c->getPhaseMsg() == START_LINE_COMPLETE)
		parseStartLine(c);
	if (c->getPhaseMsg() == HEADER_INCOMPLETE)
	{
		size_t pos1;
		if ((pos1 = c->getBuffer().find(CRLFCRLF)) != std::string::npos)
			c->setPhaseMsg(HEADER_COMPLETE);
	}
	if (c->getPhaseMsg() == HEADER_COMPLETE)
	{
		parseHeaders(c);
		checkHeader(c);
	}
}

void	RequestHandler::parseStartLine(Connection *c) {

	size_t pos = c->getBuffer().find(CRLF);
	startLine_ = c->getBuffer().substr(0, pos);
	c->getBuffer().erase(0, pos + LEN_CRLF);
	//method check : GET/POST/DELETE -> toupper / if not Error 400
	
	std::vector<std::string> split_start_line = splitDelim(startLine_, " ");
	if (split_start_line.size() != 3)
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(START_LINE_ERROR);
		return ;
	}
	if (!checkMethod(split_start_line[0]))
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(START_LINE_ERROR);
		// c->setPhaseMsg(HEADER_COMPLETE);
		return ;
	}
	else
		c->getRequest().setMethod(split_start_line[0]);

	//path check:
	c->getRequest().setUri(split_start_line[1]);
	split_start_line[1].append(" ");
	
	if (parseUri(split_start_line[1], c) == PARSE_INVALID_URI)
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(START_LINE_ERROR);
		// c->setPhaseMsg(HEADER_COMPLETE);
		return ;
	}

	//HTTP/1.1 check: 'HTTP/' '1.*' / if not Error 400/505
	std::string http = split_start_line[2].substr(0, 5);
	std::string version = split_start_line[2].substr(5, split_start_line[2].length());
	int cmp = http.compare("HTTP/");
	if (!checkVersion(version) || version.length() != 3 || cmp)
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(START_LINE_ERROR);
		// c->setPhaseMsg(HEADER_COMPLETE);
		return ;
	}
	cmp = version.compare(0, 2, "1.");
	if (cmp)
	{
		c->setReqStatusCode(HTTP_VERSION_NOT_SUPPORTED);
		c->setPhaseMsg(START_LINE_ERROR);
		// c->setPhaseMsg(HEADER_COMPLETE);
		return ;
	}
	c->getRequest().setVersion(http + version);
	c->setPhaseMsg(HEADER_INCOMPLETE);
}

int RequestHandler::parseUri(std::string uri_str, Connection *c) {
  enum {
    schema = 0,
    host,
    port,
    uri,
    args,
    uri_complete
  } state;

  if (uri_str[0] == '/')
    state = uri;
  else
    state = schema;
  size_t pos;
  while (state != uri_complete) {
    switch (state) {
      case schema:
        if ((pos = uri_str.find("://")) == std::string::npos)
          return (PARSE_INVALID_URI);
        c->getRequest().setSchema(uri_str.substr(0, pos));
        uri_str.erase(0, pos + 3);
        state = host;
        break;
      case host:
        if ((pos = uri_str.find_first_of(":/? ")) != std::string::npos)
        {
          if (pos != 0)
            c->getRequest().setHost(uri_str.substr(0, pos));
          uri_str.erase(0, pos);
        }
        else
          return (PARSE_INVALID_URI);
        switch (uri_str[0]) {
          case ':':
            state = port;
            break;
          case '/':
            state = uri;
            break;
          case '?':
            state = args;
            break;
          case ' ':
            state = uri_complete;
            break;
          default:
            return (PARSE_INVALID_URI);
        }
        break;
      case port:
        if ((pos = uri_str.find_first_of("/? ")) != std::string::npos)
        {
          for (size_t i = 1; i < pos; ++i) 
          {
            if (!isdigit(uri_str[i]))
              return (PARSE_INVALID_URI);
          }
          if (pos != 1)
            c->getRequest().setPort(uri_str.substr(1, pos - 1));
          uri_str.erase(0, pos);
        }
        else
          return (PARSE_INVALID_URI);
        switch (uri_str[0]) {
          case '/':
            state = uri;
            break;
          case '?':
            state = args;
            break;
          case ' ':
            state = uri_complete;
            break;
          default:
            return PARSE_INVALID_URI;
        }
        break;
      case uri:
        if ((pos = uri_str.find_first_of("? ")) != std::string::npos) 
        {
          if (pos != 1)
            c->getRequest().setPath(uri_str.substr(0, pos));
          uri_str.erase(0, pos);
        }
        else
          return (PARSE_INVALID_URI);
        switch (uri_str[0]) {
          case '?':
            state = args;
            break;
          case ' ':
            state = uri_complete;
            break;
          default:
            return (PARSE_INVALID_URI);
        }
        break;
      case args:
        if ((pos = uri_str.find(" ")) != std::string::npos)
        {
          if (pos != 1)
            c->getRequest().setQueryString(uri_str.substr(1, pos - 1));
          uri_str.erase(0, pos);
        }
        else
          return (PARSE_INVALID_URI);
        state = uri_complete;
        break;
      case uri_complete:
        break;
    }
  }
  if (c->getRequest().getPath().empty())
    c->getRequest().setPath("/");

  return (PARSE_VALID_URI);
}

void	RequestHandler::parseHeaders(Connection *c) {

	size_t pos = c->getBuffer().find(CRLFCRLF);
	headers_ = c->getBuffer().substr(0, pos + LEN_CRLF);
	c->getBuffer().erase(0, pos + LEN_CRLFCRLF);

	size_t pos1 = 0;
	int code = 0;
	while ((pos1 = headers_.find(CRLF)) != std::string::npos)
	{
		std::string headerline = "";
		headerline = headers_.substr(0, pos1);
		//headerline parse - argument is < 2 => 400 // if field name is not alpha => 400
													// if there's sth between fild name and colon => 400
		if (headers_.length() == 2)
			break ;
		if ((code = parseHeaderLine(c, headerline)) != NOT_DEFINE)
		{
			c->setReqStatusCode(code);
			c->setPhaseMsg(HEADER_COMPLETE);
			return ;
		}
		headers_ = headers_.substr(pos1 + LEN_CRLF, headers_.length());
	}
}

int		RequestHandler::parseHeaderLine(Connection *c, std::string headerline) {
	std::vector<std::string> header_line_parse = splitDelim(headerline, ": ");
	if (header_line_parse.size() != 2)
			return (BAD_REQUEST);
	std::string key = header_line_parse[0];
	std::string value = header_line_parse[1];
	c->getRequest().setHeader(key, value);
	return (NOT_DEFINE);
}

/* Set and check details along with header key and method of request message */
void	RequestHandler::checkHeader(Connection *c) {

	//if Request status code is set as error code, exit this function
	if (c->getReqStatusCode() != NOT_DEFINE)
	{
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}

	//check if host value is has host name and port at the same time. if so, parse.
	//if host value doesn't exist, bad request.
	if (setUriStructHostPort(c, c->getRequest().getHeaderValue("Host")) == false && c->getRequest().getVersion().compare("HTTP/1.0"))
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}
	
	//find right server block(with host name) and location block(request path) 
	c->setServerBlockConfig(c->getRequest().getHost());
	if (!c->checkLocationConfigExist(c->getRequest().getPath()))
	{
		// uri root == serverblock root
		// index.html check -> index.html use
		// if not -> autoindex check -> on => directory list / off => forbidden 403 
		c->getRequest().setFilePath(c->getServerConfig().getRoot() + c->getRequest().getPath());
		if (!(*(c->getRequest().getFilePath().rbegin()) == '/') && isUriDirectory(c))
			c->getRequest().setFilePath(c->getRequest().getFilePath() + "/");
		if (!checkIndex(c))
		{
			if (c->getServerConfig().getAutoindex() == false)
			{
				if (isFileExist(c) && isUriDirectory(c))
				{
					c->setReqStatusCode(FORBIDDEN); //403 FORBIDDEN
					c->setPhaseMsg(BODY_COMPLETE);
					return ;
				}
				else if (!isFileExist(c))
				{
					c->setReqStatusCode(NOT_FOUND); //404 FORBIDDEN
					c->setPhaseMsg(BODY_COMPLETE);
					return ;
				}
			}
			else
				c->autoindex_flag = true;
		}
		
	}
	else //set path / file path / uri
		setFilePathWithLocation(c->getLocationConfig(), c);

	
	//get Client_Max_Body_Size
	c->client_max_body_size = c->getServerConfig().getClientMaxBodySize();

	if ((c->getRequest().getRequestHeaders().count("Content-Length")) && !c->getRequest().getHeaderValue("Content-Length").empty())
	{
		c->buffer_content_length = fromString<size_t>((c->getRequest().getHeaderValue("Content-Length")));
		if ((size_t)c->buffer_content_length > (c->client_max_body_size))
		{
			c->setReqStatusCode(PAYLOAD_TOO_LARGE);
			c->setPhaseMsg(BODY_COMPLETE);
			return ;
		}
		if (!(c->getBuffer().empty())) 	//put the rest of buffer into request body member
			c->setBodyBuf(c->getBuffer());
	}

	// when file doesn't exist
	if (c->getRequest().getMethod() != "POST" && !isFileExist(c))
	{
		c->setReqStatusCode(NOT_FOUND);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}

	// when index directive exist
	if (*(c->getRequest().getFilePath().rbegin()) == '/' && c->getRequest().getMethod().compare("DELETE"))
	{
		if (!checkIndex(c))
		{
			if (c->getServerConfig().getAutoindex() == false)
			{
				if (!c->getLocationConfig().getReturn().empty())
				{
					checkLocationReturnAndApply(c->getLocationConfig().getReturn(), c);
					c->setPhaseMsg(BODY_COMPLETE);
					return ;
				}
				c->setReqStatusCode(FORBIDDEN); //403 FORBIDDEN
				c->setPhaseMsg(BODY_COMPLETE);
				return ;
			}
			else
				c->autoindex_flag = true;
		}
	}

	//Allow method check
	if (checkAllowMethod(c) == false)
	{
		c->setReqStatusCode(METHOD_NOT_ALLOWED);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}

	//location config return value check
	if (!c->getLocationConfig().getReturn().empty())
	{
		checkLocationReturnAndApply(c->getLocationConfig().getReturn(), c);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}
	
	if((isUriDirectory(c) == true) && (c->getRequest().getMethod() == "DELETE") && (c->getLocationConfig().getAutoindex() == false))
	{
		c->setReqStatusCode(MOVED_PERMANENTLY);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}

	if (c->getRequest().getMethod() == "DELETE")
	{
		struct stat stat_buffer;

		stat(c->getRequest().getFilePath().c_str(), &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode) && *(c->getRequest().getFilePath().rbegin()) != '/')
		{
			c->setReqStatusCode(CONFLICT);
			c->setPhaseMsg(BODY_COMPLETE);
			return ;
		}
		else if (c->getRequest().getPath() == "/")
		{
			c->setReqStatusCode(FORBIDDEN);
			c->setPhaseMsg(BODY_COMPLETE);
			return ;
		}
	}

	//chunked message flag on/off
	if ((c->getRequest().getRequestHeaders().count("Transfer-Encoding")) && !(c->getRequest().getHeaderValue("Transfer-Encoding")).compare("chunked"))
	{
		c->is_chunk = true;
		c->setPhaseMsg(BODY_CHUNKED);
	}
	else if (c->getRequest().getMethod() == "GET" || c->getRequest().getMethod() == "DELETE")
	{
		c->is_chunk = false;
		c->getBodyBuf().clear();
		c->buffer_content_length = -1;
		c->setPhaseMsg(BODY_COMPLETE);
	}
	else if (c->is_chunk == true)
		c->setPhaseMsg(BODY_COMPLETE);
	else if (c->is_chunk == false && c->buffer_content_length != -1 && c->buffer_content_length <= (int)c->getBodyBuf().size())
		c->setPhaseMsg(BODY_COMPLETE);
	else
		c->setPhaseMsg(BODY_INCOMPLETE);
}

void	RequestHandler::checkRequestBody(Connection *c) {

	if (c->is_chunk == false)
	{
		if (((ssize_t)c->buffer_content_length == fromString<ssize_t>(c->getRequest().getHeaderValue("Content-Length"))) && !strcmp("\r\n", c->buffer_char))
			return ;
		
		else if ((size_t)c->buffer_content_length <= strlen(c->buffer_char)) //buffer content length is shorter or the same as buffer char
		{
			c->body_buf.append(c->buffer_char, c->buffer_content_length);
			c->buffer_content_length = -1;
			c->getRequest().setBody(c->body_buf);
			c->setPhaseMsg(BODY_COMPLETE);
		}
		else //buffer content length is longer buffer char
		{
			c->buffer_content_length = c->buffer_content_length - strlen(c->buffer_char);
			c->setBodyBuf(c->buffer_char);
		}
	}
	else
		c->setPhaseMsg(BODY_COMPLETE);
}

bool	RequestHandler::checkChunkedMessage(Connection *c) {
	size_t pos;

	while ((pos = c->getBuffer().find(CRLF)) != std::string::npos) // loop until CRLF exist in buffer
	{
		if (c->chunked_msg_checker == STR_SIZE) //when number is STR_SIZE 
		{
			if ((pos = c->getBuffer().find(CRLF)) != std::string::npos)
			{
				if (c->client_max_body_size < c->getBodyBuf().length()) 
				{
					c->getBodyBuf().clear();
					c->setReqStatusCode(PAYLOAD_TOO_LARGE);
					c->setPhaseMsg(BODY_COMPLETE);
					c->is_chunk = false;
					return (true);
				}
			}
			//str to unsigned long
			c->chunked_msg_size = (size_t)strtoul(c->getBuffer().substr(0, pos).c_str(), NULL, 16);
			if (c->getReqStatusCode() != NOT_DEFINE && c->chunked_msg_size != 0)
				return (false);
			if (c->chunked_msg_size == 0)
			{
				if (c->getBuffer()[0] != '0' && pos == 0) {
          		c->getBodyBuf().clear();
							c->setReqStatusCode(BAD_REQUEST);
							c->setPhaseMsg(BODY_COMPLETE);
							c->is_chunk = false;
							return (true);
        		}
				for (size_t i = 0; i < pos; ++i)
				{
					if (c->getBuffer()[i] != '0')
					{
						if (c->getReqStatusCode() == NOT_DEFINE)
						{
							c->getBodyBuf().clear();
							c->setReqStatusCode(BAD_REQUEST);
							c->setPhaseMsg(BODY_COMPLETE);
							c->is_chunk = false;
							return (true);
						}
						else
							return (false);
					}
				}
			}
			c->getBuffer().erase(0, pos + 2);
			if (c->chunked_msg_size == 0)
				c->chunked_msg_checker = END;
			else
				c->chunked_msg_checker = STR;
		}
		if (c->chunked_msg_checker == STR) // string
		{
			if (c->getBuffer().size() >= (c->chunked_msg_size + 2) && !(c->getBuffer().compare(c->chunked_msg_size, 2, CRLF)))
			{
				c->body_buf.append((char *)c->getBuffer().c_str(), c->chunked_msg_size);
				c->getBuffer().erase(0, c->chunked_msg_size + LEN_CRLF);
				c->chunked_msg_checker = STR_SIZE;
			}
			if (c->getBuffer().size() >= c->chunked_msg_size + 4)
			{
				c->getBodyBuf().clear();
				c->setReqStatusCode(BAD_REQUEST);
				c->setPhaseMsg(BODY_COMPLETE);
				return (true);
			}
			else
				return (true);
		}
		if (c->chunked_msg_checker == END) // At the end of chunked message
		{
			if ((pos = c->getBuffer().find(CRLF)) == 0)
			{
				c->getBuffer().clear();
				if (c->getReqStatusCode() != NOT_DEFINE)
				{
					c->setPhaseMsg(START_LINE_INCOMPLETE);
					c->setReqStatusCode(NOT_DEFINE);
				}
				else
					c->setPhaseMsg(BODY_COMPLETE);
				c->is_chunk = false;
			}
			else if (pos != std::string::npos)
				c->getBuffer().erase(0, pos + LEN_CRLF);
		}
	}
	return (true);
}

//utiles
std::vector<std::string> RequestHandler::splitDelim(std::string s, std::string delim) {
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

int		RequestHandler::checkMethod(const std::string &s) {

	for(size_t i = 0; i < s.length(); i++)
	{
		char c = s[i];
		if (!isupper(c) || !isalpha(c))
			return (false);
	}
	return (true);
}

int			RequestHandler::checkVersion(const std::string &s) {
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

int			RequestHandler::checkHeaderKey(const std::string &s) {
	for (size_t i = 0; i < s.length(); i++)
	{
		char c = s[i];
		if (isalpha(c) || (c == '_'))
			i++;
		else
			return (false);
	}
	return (true);
}

bool RequestHandler::isFileExist(Connection *c) {
	struct stat stat_buf;

	if (stat(c->getRequest().getFilePath().c_str(), &stat_buf) == -1)
		return (false);
	return (true);
}

void	RequestHandler::setFilePathWithLocation(LocationBlock location, Connection *c) {

	//example: request path : /test/
	std::string filepath;
	filepath = location.getRoot(); //filepath init /var/www/html/cgi_tester in to location root

	/*	block root: /var/www/html
		location root(filepath): /var/www/html/cgi_tester
		case1)	location uri: /cgi_tester/
				request path: /cgi_tester
		case2)	location uri: /cgi_tester/
				request path: /cgi_tester/test1		*/
	if (location.getRoot() != c->getBlock()[0].getRoot())
	{
		if (c->getRequest().getPath().substr(location.getUriPath().length()).empty()) //case 1
			filepath.append("/");
		else // case 2
		{
			if (*(location.getUriPath().rbegin()) == '/') // /cgi_tester/
				filepath.append(c->getRequest().getPath().substr(location.getUriPath().length() - 1));
			else // /cgi_tester
				filepath.append(c->getRequest().getPath().substr(location.getUriPath().length()));
		}
	}
	else
	{
	/* 	block root: /var/www/html
		location root: /var/www/html */
		filepath.append(c->getRequest().getPath()); //just append the rest of path on request path to complet path
		// if (*(location.getUriPath().rbegin()) != '/')
		// 	filepath.append("/");
	}
	if (*(filepath.rbegin()) != '/' && isUriDirectory(filepath))
		filepath.append("/");
	c->getRequest().setFilePath(filepath);
}

int	RequestHandler::setUriStructHostPort(Connection *c, std::string host_value) {
	std::vector<std::string> host_value_parse = splitDelim(host_value, ":");
	
	if (host_value.empty())
		return (false);
	if (host_value_parse.size() == 2)
	{
		c->getRequest().setHost(host_value_parse[0]);
		c->getRequest().setPort(host_value_parse[1]);
	}
	else
		c->getRequest().setHost(host_value_parse[0]);
	return (true);
}

bool	RequestHandler::checkIndex(Connection *c) {
	std::vector<std::string> index = c->getServerConfig().getIndex();
	struct stat stat_buf;
	std::string index_path;

	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++)
	{
		index_path = c->getRequest().getFilePath() + (*it);
		if (stat(index_path.c_str(), &stat_buf) == 0)
		{
			c->getRequest().setFilePath(index_path);
			return (true);
		}
		index_path.clear();
	}
	return (false);
}

bool	RequestHandler::checkAllowMethod(Connection *c) {
	std::vector<std::string> limit_method = c->getLocationConfig().getLimitExcept();
	if (limit_method.size() == 0)
		return (true);
	for(size_t i = 0; i < limit_method.size(); i++)
	{
		if (limit_method[i] == c->getRequest().getMethod())
			return (true);
	}
	return (false);
}

void	RequestHandler::checkLocationReturnAndApply(std::vector<std::string> ret, Connection *c) {
	(void)c;
	size_t 		code = 0;
	std::string str = "";

	for(size_t i = 0; i < ret.size(); i++)
	{
		if (isdigit(*(ret[i].begin())))
			code = fromString<size_t>(ret[i]);
		else
			str = ret[i];
	}
	if (code == 301 || code == 302 || code == 303 || code == 307 || code == 308)
	{
		c->setReqStatusCode(code);
		if (!str.empty())
			c->getRequest().setHeader("Location", str);
		else
			c->getRequest().setHeader("Location", " ");
		return ;
	}
	if (!str.empty())
		c->setBodyBuf(str);
	c->setReqStatusCode(code);
}

bool RequestHandler::isUriDirectory(Connection *c) {
	struct stat stat_buffer;

	stat(c->getRequest().getFilePath().c_str(), &stat_buffer);
	if (S_ISDIR(stat_buffer.st_mode))
		return (true);
	return (false);
}


bool RequestHandler::isUriDirectory(std::string path) {
	struct stat stat_buffer;

	stat(path.c_str(), &stat_buffer);
	if (S_ISDIR(stat_buffer.st_mode))
		return (true);
	return (false);
}