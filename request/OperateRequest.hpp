#ifndef OPERATE_REQUEST_HPP
# define OPERATE_REQUEST_HPP

# include "connection.hpp"
# include "request.hpp"
# include "utile.cpp"

class Connection;
class Request;

class OperateRequest {

	private:
		Request		*request_;
		std::string	startLine_;
		std::string	headers_;
		size_t		tmp_;

	public:
		OperateRequest(void);
		// OperateRequest(Connection *c);
		~OperateRequest(void);

		//getter
		std::string	&getStartLine(void);
		std::string	&getHeaders(void);

		//setter
		void	setRequest(Request *request);

		void	checkRequestMessage(Connection *c);
		void	parseStartLine(Connection *c);
		std::vector<std::string> splitDelim(std::string s, std::string delim);

};

#endif
