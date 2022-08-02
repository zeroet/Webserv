#include "OperateRequest.hpp"

OperateRequest::OperateRequest(void) {}

OperateRequest::~OperateRequest(void) {}

// OperateRequest::OperateRequest(Connection *c) : request_(&c->getRequest()) {}

void	OperateRequest::setRequest(Request *req) {
	request_ = req;
}

void	OperateRequest::checkRequestMessage(Connection *c) {

	size_t pos = 0;
	if (c->getPhaseMsg() == START_LINE_INCOMPLETE)
	{
		if ((pos = c->getBuffer().find(CRLF))!= std::string::npos)
		{
			c->setPhaseMsg(START_LINE_COMPLETE);
			startLine_ = c->getBuffer().substr(0, pos);
			tmp_ = pos + 1;
		}
	}
	// if (pos2 != 0)
	if (c->getPhaseMsg() == START_LINE_COMPLETE)
	{
		//parse start line
		std::cout << "Parse Start Line" << std::endl;
		if ((pos = c->getBuffer().find(CRLFCRLF)) == std::string::npos)
			c->setPhaseMsg(HEADER_INCOMPLETE);
	}
	if (c->getPhaseMsg() == HEADER_INCOMPLETE)
	{
	// 	size_t pos = 0;
		if ((pos = c->getBuffer().find(CRLFCRLF)) != std::string::npos)
		{
			c->setPhaseMsg(HEADER_COMPLETE);
			headers_ = c->getBuffer().substr(tmp_, c->getBuffer().length());
			tmp_ = pos + 1;
			std::cout << "header: " << headers_ << std::endl;
			std::cout << "tmp_: " << tmp_ << std::endl;
		}
	}
	if (c->getPhaseMsg() == HEADER_COMPLETE)
	{
		//parse header
		std::cout << "Parse Header" << std::endl;
	}
}

void	OperateRequest::parseStartLine(Connection *c) {
	(void)c;
}

//getter
std::string	&OperateRequest::getStartLine(void) {
	return (startLine_);
}
std::string	&OperateRequest::getHeaders(void) {
	return (headers_);
}
