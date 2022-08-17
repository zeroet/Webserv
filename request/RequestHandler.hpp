#pragma once

# include "../server/connection.hpp"
# include "request.hpp"

# include <cctype>
# include <string>
# include <algorithm>
# include <sstream>
# include <limits>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

namespace ft {

// class Request;
class Connection;

#define PARSE_VALID_URI 1
#define PARSE_INVALID_URI 0

/*
 * Class to operate the validation and the parsing of http request message
 * After this process, the result will be transfered and stocked into Request class which located in Connection class
 */
class RequestHandler {

	private:
		std::string	startLine_;	//extract of start line from buffer
		std::string	headers_;	//extract of headers from buffer
		std::string body_;
		size_t		tmp_;		//for stock end pos of buffer(from connection)

		// ServerBlock	*serverConfig_;

	public:
		RequestHandler(void);
		~RequestHandler(void);

		//getter
		std::string	&getStartLine(void);
		std::string	&getHeaders(void);
		std::string	&getBody(void);

		//setter
		void	setFilePathWithLocation(LocationBlock location, Connection *c);
		void	setServerConfig(std::string server_name);
		void	setBody(std::string bodybuf_);
		
		void	checkRequestMessage(Connection *c);
		void	parseStartLine(Connection *c);
		void	parseHeaders(Connection *c);
		int		parseHeaderLine(Connection *c, std::string headerline);
		void	checkHeader(Connection *c);
		int		parseUri(std::string uri, Connection *c);
		void	checkRequestBody(Connection *c);
		bool	checkIndex(Connection *c);
		bool	checkChunkedMessage(Connection *c);
		

		//utiles
		std::vector<std::string> splitDelim(std::string s, std::string delim);
		int		checkMethod(const std::string &s);
		int		checkVersion(const std::string &s);
		int		checkHeaderKey(const std::string &s);
		// int		checkHostHeader(Connection *c);
		bool	isFileExist(Connection *c);
		int		setUriStructHostPort(Connection *c, std::string host_value);
		bool	checkAllowMethod(Connection *c);
		void	checkLocationReturnAndApply(std::vector<std::string> ret, Connection *c);
		bool	isUriDirectory(Connection *c);

		template<typename T>
		std::string toString(const T& v)
		{
			std::ostringstream ss;
			ss << v;
			return (ss.str());
		}

		template<typename T>
		T fromString(const std::string& str)
		{
			std::istringstream ss(str);
			T ret;
			ss >> ret;
			return (ret);
		}

};

}

