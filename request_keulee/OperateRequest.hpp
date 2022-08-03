#pragma once

# include "connection.hpp"
# include "request.hpp"

# include <cctype>
#include <string>
#include <algorithm>

class Connection;
class Request;

/*
 * Class to operate the validation and the parsing of http request message
 * After this process, the result will be transfered and stocked into Request class which located in Connection class
 */
class OperateRequest {

	private:
		std::string	startLine_;	//extract of start line from buffer
		std::string	headers_;	//extract of headers from buffer
		size_t		tmp_;		//for stock end pos of buffer(from connection)

	public:
		OperateRequest(void);
		~OperateRequest(void);

		//getter
		std::string	&getStartLine(void);
		std::string	&getHeaders(void);

		//setter

		void	checkRequestMessage(Connection *c);
		void	parseStartLine(Connection *c);
		void	parseHeaders(Connection *c);
		int		parseHeaderLine(Connection *c, std::string headerline);
		void	checkRequestHeader(Connection *c);

		//utiles
		std::vector<std::string> splitDelim(std::string s, std::string delim);
		int		checkMethod(const std::string &s);
		int		checkVersion(const std::string &s);
		int		checkHeaderValue(const std::string &s);
		std::string	trimWhiteSpace(std::string &s);

};
