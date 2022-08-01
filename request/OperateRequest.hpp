#ifndef OPERATE_REQUEST_HPP
# define OPERATE_REQUEST_HPP

# include "connection.hpp"
# include "request.hpp"

class Connection;
class Request;

class OperateRequest {

	private:
		Request *request_;

	public:
		OperateRequest(void);
		~OperateRequest(void);

		//setter
		void	setRequest(Request *request);

		static void	checkRequestMessage(Connection *c);

};

#endif
