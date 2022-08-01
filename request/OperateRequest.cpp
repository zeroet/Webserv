#include "OperateRequest.hpp"

OperateRequest::OperateRequest(void) {}

OperateRequest::~OperateRequest(void) {}

void	OperateRequest::setRequest(Request *req) {
	request_ = req;
}

void	OperateRequest::checkRequestMessage(Connection *c) {

	size_t pos = 0;
	if (c->getPhaseMsg() == START_LINE_INCOMPLETE)
	{
		if ((pos = c->getBuffer().find(CRLF) )!= std::string::npos)
		{
			// std::cout << "pos: " << pos << std::endl;
			// std::cout << getBuffer().substr(pos, ) << std::endl;
			c->setPhaseMsg(START_LINE_COMPLETE);
		}
	}
	// if (pos2 != 0)
	if (c->getPhaseMsg() == START_LINE_COMPLETE)
	{
		//parse start line
		std::cout << "Parse Start Line" << std::endl;
	// 	// if ((pos = getBuffer().find(CRLFCRLF)) != std::string::npos)
	// 	// std::cout << pos2 << std::endl;
	// 	// std::cout << getBuffer().substr(pos2, getBuffer().length()) << std::endl;
		if ((pos = c->getBuffer().find(CRLFCRLF)) == std::string::npos)
			c->setPhaseMsg(HEADER_INCOMPLETE);
	}
	if (c->getPhaseMsg() == HEADER_INCOMPLETE)
	{
	// 	size_t pos = 0;
		if ((pos = c->getBuffer().find(CRLFCRLF)) != std::string::npos)
		{
			// std::cout << "pos: " << pos << std::endl;
	// 	std::cout << getBuffer().substr(pos2, getBuffer().length()) << std::endl;
			c->setPhaseMsg(HEADER_COMPLETE);
		}
	}
	if (c->getPhaseMsg() == HEADER_COMPLETE)
	{
		//parse header
		std::cout << "Parse Header" << std::endl;
	}
}
