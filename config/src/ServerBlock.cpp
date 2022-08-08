#include "ServerBlock.hpp"

namespace ft
{
	ServerBlock::ServerBlock(const BaseDirectives& context)
	{
		this->root_ = context.getRoot();
		this->client_max_body_size_ = context.getClientMaxBodySize();
		this->keepalive_timeout_ = context.getKeepaliveTimeout();
		this->index_ = context.getIndex();
		this->autoindex_ = context.getAutoindex();
		this->error_page_ = context.getErrorPage();
		//setErrorPage(context.getErrorPage());
		this->listen_ = 80;
		this->server_name_.push_back("");
		this->socket_fd_ = -1;
	}

	// Getter
	const std::vector<LocationBlock>	ServerBlock::getLocationBlock() const
	{
		return (this->location_list);
	}

	const std::pair<bool, LocationBlock>	ServerBlock::getLocationBlock(const std::string path) const
	{
		LocationBlock	return_location;
		
		(void)path;
		return (std::make_pair(false, return_location));
	}

	unsigned int		ServerBlock::getListen(void) const
	{
		return (this->listen_);
	}

	const std::vector<std::string>	ServerBlock::getServerName(void) const
	{
		return (this->server_name_);
	}

	const std::string		ServerBlock::getServerName(size_t index) const
	{
		return (this->server_name_.at(index));
	}

	const std::vector<std::string>	ServerBlock::getReturn(void) const
	{
		return (this->return_);
	}

	int	ServerBlock::getSocketFd(void) const
	{
		return (this->socket_fd_);
	}

	// Setter
	void				ServerBlock::setListen(const unsigned int x)
	{
		this->listen_ = x;
	}

	void				ServerBlock::setServerName(const std::string x)
	{
		this->server_name_.push_back(x);
	}

	void				ServerBlock::setReturn(const std::string x)
	{
		this->return_.push_back(x);
	}

	void				ServerBlock::setSocketFd(const int i)
	{
		this->socket_fd_ = i;
	}

	void				ServerBlock::clearServerName(void)
	{
		this->server_name_.clear();
	}
}
