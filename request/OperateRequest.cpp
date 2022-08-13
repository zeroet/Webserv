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

void	OperateRequest::setBody(std::string bodybuf_) {
	body_ = bodybuf_;
}

void	OperateRequest::checkRequestMessage(Connection *c) {

	if (c->getPhaseMsg() == START_LINE_INCOMPLETE)
	{
		size_t pos;
		if ((pos = c->getBuffer().find(CRLF))!= std::string::npos)
			c->setPhaseMsg(START_LINE_COMPLETE);
	}
	if (c->getPhaseMsg() == START_LINE_COMPLETE)
	{
		//parse start line
		std::cout << "Parse Start Line" << std::endl;
		parseStartLine(c);
	}
	if (c->getPhaseMsg() == HEADER_INCOMPLETE)
	{
		size_t pos1;
		if ((pos1 = c->getBuffer().find(CRLFCRLF)) != std::string::npos)
			c->setPhaseMsg(HEADER_COMPLETE);
	}
	if (c->getPhaseMsg() == HEADER_COMPLETE)
	{
		std::cout << "Parse Header" << std::endl;
		parseHeaders(c);
	// 	std::cout << "Header parse done" << std::endl;
	// 	std::cout << "buffer check: " << c->getBuffer() << std::endl;
		checkHeader(c);
	}
}

void	OperateRequest::parseStartLine(Connection *c) {

	size_t pos = c->getBuffer().find(CRLF);
	startLine_ = c->getBuffer().substr(0, pos);
	c->getBuffer().erase(0, pos + LEN_CRLF);
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
	c->getRequest().setUri(split_start_line[1]);
	split_start_line[1].append(" ");
	
	if (parseUri(split_start_line[1], c) == PARSE_INVALID_URI)
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(HEADER_COMPLETE);
		return ;
	}

	//HTTP/1.1 check: 'HTTP/' '1.*' / if not Error 400/505
	std::string http = split_start_line[2].substr(0, 5);
	std::string version = split_start_line[2].substr(5, split_start_line[2].length());
	int cmp = http.compare("HTTP/");
	if (!checkVersion(version) || version.length() != 3 || cmp)
	{
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

int OperateRequest::parseUri(std::string uri_str, Connection *c) {
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

void	OperateRequest::parseHeaders(Connection *c) {

	size_t pos = c->getBuffer().find(CRLFCRLF);
	headers_ = c->getBuffer().substr(0, pos + LEN_CRLF);
	c->getBuffer().erase(0, pos + LEN_CRLFCRLF);
	// std::cout << "headers" << std::endl;
	// std::cout << headers_ << std::endl;
	// size_t header_len = header.length();
	size_t pos1 = 0;
	int code = 0;
	// int i = 0;
	while ((pos1 = headers_.find(CRLF)) != std::string::npos)
	{
		std::string headerline = "";
		headerline = headers_.substr(0, pos1);
		// std::cout << "Header line " << i++ << " : " << headerline << std::endl;
		// }

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
		// std::cout << "headers to parse line : " << std::endl;
		// std::cout << headers_ << std::endl;
		// std::cout << "header 길이: " << headers_.length() << std::endl;
	}
	// std::cout <<
}

int		OperateRequest::parseHeaderLine(Connection *c, std::string headerline) {
	std::vector<std::string> header_line_parse = splitDelim(headerline, ": ");
	if (header_line_parse.size() != 2)
	{
		// header_line_parse = splitDelim(headerline, ":");
		// if (header_line_parse.size() != 2)
		std::cout << header_line_parse.size() << std::endl;
		std::cout<< "/////////////////////////////////////////////" << std::endl;
			return (BAD_REQUEST);
	}
	// std::vector<std::string> key_parse = splitDelim(header_line_parse[0], ":");

	// std::string key = key_parse[0];
	std::string key = header_line_parse[0];
	std::string value = header_line_parse[1];
	// std::cout << "header key: " << key << std::endl;
	// std::cout << "header value: " << value << std::endl;
	c->getRequest().setHeader(key, value);
	return (NOT_DEFINE);
}

/* Set and check details along with header key and method of request message */
void	OperateRequest::checkHeader(Connection *c) {

	//check if host value is has host name and port at the same time. if so, parse.
	//if host value doesn't exist, bad request.
	if (setUriStructHostPort(c, c->getRequest().getHeaderValue("Host")) == false)
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}
	
	//find right server block(with host name) and location block(request path) 
	c->setServerBlockConfig(c->getRequest().getHost());
	if (!c->checkLocationConfigExist(c->getRequest().getPath()))
	{
		c->setReqStatusCode(NOT_FOUND);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}

	//if Request status code is set as error code, exit this function
	if (c->getReqStatusCode() != NOT_DEFINE)
	{
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}

	//host header must exist when HTTP/1.* (HTTP/1.0 header doesn't need)
	if (checkHostHeader(c) == false)
	{
		c->setReqStatusCode(BAD_REQUEST);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}

	// location block test
	// std::cout << "location root:  << c->getLocationConfig().getRoot() << std::endl;

	//set path / file path / uri
	setFilePathWithLocation(c->getLocationConfig(), c);

	//get Client_Max_Body_Size
	c->client_max_body_size = c->getServerConfig().getClientMaxBodySize();

	if ((c->getRequest().getRequestHeaders().count("Content-Length")) && !c->getRequest().getHeaderValue("Content-Length").empty())
	{
		c->content_length = fromString<size_t>((c->getRequest().getHeaderValue("Content-Length")));
		std::cout << "content-length in function:" << c->content_length << std::endl;
		if (c->content_length > (c->client_max_body_size))
		{
			c->setReqStatusCode(PAYLOAD_TOO_LARGE);
			c->setPhaseMsg(BODY_COMPLETE);
			return ;
		}
		if (!(c->getBuffer().empty())) 	//put the rest of buffer into request body member
		{
			c->setBodyBuf(c->getBuffer());
			std::cout << "////////////////body buf check: " << c->getBodyBuf() << std::endl;
			// c->getBuffer().erase(0, c->getBuffer().length());
			c->getRequest().setBody(c->getBodyBuf());
			// c->setBodyBuf(body_);
			// c->getBuffer().erase(0, c->getBuffer().length());
		}
		else
			std::cout << "///////////////////////////EMPTY////////////////////////" << std::endl;
	}
	else if ((c->getRequest().getRequestHeaders().count("Transfer-Encoding")) && !(c->getRequest().getHeaderValue("Transfer-Encoding")).compare("Chunked"))
		c->is_chunk = true;

	// when file doesn't exist
	if (c->getRequest().getMethod() == "GET" && !isFileExist(c))
	{
		c->setReqStatusCode(NOT_FOUND);
		c->setPhaseMsg(BODY_COMPLETE);
		return ;
	}
	
	//chunked message flag on/off
	if (c->getRequest().getMethod() == "GET" || c->getRequest().getMethod() == "DELETE")
	{
		c->is_chunk = false;
		c->content_length = 0;
		c->setPhaseMsg(BODY_COMPLETE);
	}
	else if (c->getRequest().getRequestHeaders().count("Transfer-Encoding") && (c->getRequest().getHeaderValue("Transfer-Encoding") == "chunked"))
	{
		c->setPhaseMsg(BODY_CHUNKED);
		c->is_chunk = true;
	}
	else if (c->is_chunk == true)
		c->setPhaseMsg(BODY_COMPLETE);
	else
		c->setPhaseMsg(BODY_INCOMPLETE);

	//location config return value check
	
	//Allow method check

	//directory exist check

	//When Method is delete try to delete directory -> no no
}

void	OperateRequest::checkRequestBody(Connection *c) {
	(void)c;
	std::cout << "+++++++++++++++MANAGING BODY+++++++++++++++++" << std::endl;
	std::cout << "status chunk ? " << c->is_chunk << std::endl;
	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	if (!c->is_chunk)
	{
		if (((ssize_t)c->content_length == fromString<ssize_t>(c->getRequest().getHeaderValue("Content-Length"))) && !strcmp("\r\n", c->buffer_char))
		{
			std::cout << "&&&&&&&&&&&&& HERE 1 &&&&&&&&&&&&&&" << std::endl;
			return ;
		}
		else if ((size_t)c->content_length <= strlen(c->buffer_char))
		{
			std::cout << "&&&&&&&&&&&&& HERE 2 &&&&&&&&&&&&&&" << std::endl;
			c->body_buf.append(c->buffer_char, c->content_length);
			c->content_length = 0;
			c->setPhaseMsg(BODY_COMPLETE);
		}
		else
		{
			std::cout << "&&&&&&&&&&&&& HERE 3 &&&&&&&&&&&&&&" << std::endl;
			c->content_length = c->content_length - strlen(c->buffer_char);
			c->setBodyBuf(c->buffer_char);
		}

	}
	else
		c->setPhaseMsg(BODY_COMPLETE);
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

int			OperateRequest::checkHeaderKey(const std::string &s) {
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

// std::string	OperateRequest::trimWhiteSpace(std::string &s) {

// 	std::string str;
// 	std::string whitespace(" \n\r\t\f\v");
// 	size_t found1 = s.find_first_not_of(whitespace);
// 	size_t found2 = s.find_last_not_of(whitespace);
// 	str = s.substr(found1, found2 - found1 + 1);
// 	return (str);
// }

// int Stoi(const std::string &str, size_t *idx, int base) {
//   char *end;
//   const char *p = str.c_str();
//   long num = std::strtol(p, &end, base);
//   if (p == end) {
//     throw std::invalid_argument("Stoi");
//   }
//   if (num < std::numeric_limits<int>::min() ||
//       num > std::numeric_limits<int>::max() || errno == ERANGE) {
//     throw std::out_of_range("Stoi");
//   }
//   if (idx != NULL) {
//     *idx = static_cast<size_t>(end - p);
//   }
//   return static_cast<int>(num);
// }

bool	OperateRequest::checkHostHeader(Connection *c) {
	if ((c->getRequest().getRequestHeaders().count("Host")) && (c->getRequest().getVersion().compare(0, 7, "HTTP/1.") == 0))
		return (true);
	else if (c->getRequest().getVersion().compare("HTTP/1.0") == 0)
		return (true);
	return (false);
}

bool OperateRequest::isFileExist(Connection *c) {
	struct stat stat_buf;

	if (stat(c->getRequest().getFilePath().c_str(), &stat_buf) == -1)
		return (false);
	return (true);

}

void	OperateRequest::setFilePathWithLocation(LocationBlock location, Connection *c) {

	//example: request path : /test/
	std::string filepath;
	filepath = location.getRoot(); //location root로 filepath init /var/www/html/cgi_tester
	// std::cout << "location root: " << filepath << std::endl;
	// std::cout << "block root: " << c->getBlock().getRoot() << std::endl;

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
	/* 	block root: /var/www/html
		location root: /var/www/html */
		filepath.append(c->getRequest().getPath()); //just append the rest of path on request path to complet path
	c->getRequest().setFilePath(filepath);
}

int	OperateRequest::setUriStructHostPort(Connection *c, std::string host_value) {
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
