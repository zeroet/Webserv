#include "ServerBlock.hpp"

namespace ft
{
	ServerBlock::ServerBlock(const BaseDirectives& context)
	{
		this->root_ = context.getRoot();
		this->clientMaxBodySize_ = context.getClientMaxBodySize();
		this->keepaliveTimeout_ = context.getKeepaliveTimeout();
		this->index_ = context.getIndex();
		this->autoindex_ = context.getAutoindex();
		this->errorPage_ = context.getErrorPage();
		this->listen_ = 80;
		this->serverName_.push_back("");
		this->socketFd_ = -1;
	}

	// Getter
/*
	const std::vector<LocationBlock>			getLocationBlock()
	{
		return (this->locationList);
	}
*/

	unsigned int		ServerBlock::getListen(void) const
	{
		return (this->listen_);
	}

	const std::vector<std::string>	ServerBlock::getServerName(void) const
	{
		return (this->serverName_);
	}

	const std::vector<std::string>	ServerBlock::getReturn(void) const
	{
		return (this->return_);
	}

	int	ServerBlock::getSocketFd(void) const
	{
		return (this->socketFd_);
	}

	// Setter
	void				ServerBlock::setListen(const unsigned int x)
	{
		this->listen_ = x;
	}

	void				ServerBlock::setServerName(const std::string x)
	{
		this->serverName_.push_back(x);
	}

	void				ServerBlock::setReturn(const std::string x)
	{
		this->return_.push_back(x);
	}

	void				ServerBlock::setSocketFd(const int i)
	{
		this->socketFd_ = i;
	}

	void				ServerBlock::clearServerName(void)
	{
		this->serverName_.clear();
	}
}
