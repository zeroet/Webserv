#include "ServerBlock.hpp"

namespace ft
{
	ServerBlock::ServerBlock()
	{
	}

	ServerBlock::ServerBlock(const ServerBlock& other) : location_list(other.getLocationBlock())
	{
		this->setRoot(other.getRoot());
		this->setClientMaxBodySize(other.getClientMaxBodySize());
		this->setKeepaliveTimeout(other.getKeepaliveTimeout());
		this->setIndex(other.getIndex());
		this->setAutoindex(other.getAutoindex());
		this->setErrorPage(other.getErrorPage());
		this->setConfigPath(other.getConfigPath());
		this->setListen(other.getListen());
		this->setServerName(other.getServerName());
		this->setSocketFd(other.getSocketFd());
		this->setReturn(other.getReturn());
	}

	ServerBlock::ServerBlock(const BaseDirectives& context)
	{
		this->root_ = context.getRoot();
		this->client_max_body_size_ = context.getClientMaxBodySize();
		this->keepalive_timeout_ = context.getKeepaliveTimeout();
		this->index_ = context.getIndex();
		this->autoindex_ = context.getAutoindex();
		this->error_page_ = context.getErrorPage();
		this->config_path_ = context.getConfigPath();
		this->listen_ = 80;
		this->server_name_.push_back("");
		this->socket_fd_ = -1;
	}

	ServerBlock::~ServerBlock()
	{
	}

	ServerBlock& ServerBlock::operator= (const ServerBlock& other)
	{
		if (this != &other)
		{
			this->setRoot(other.getRoot());
			this->setClientMaxBodySize(other.getClientMaxBodySize());
			this->setKeepaliveTimeout(other.getKeepaliveTimeout());
			this->setIndex(other.getIndex());
			this->setAutoindex(other.getAutoindex());
			this->setErrorPage(other.getErrorPage());
			this->setConfigPath(other.getConfigPath());
			this->setListen(other.getListen());
			this->setServerName(other.getServerName());
			this->setSocketFd(other.getSocketFd());
			this->setReturn(other.getReturn());
			this->location_list = other.getLocationBlock();
		}
		return (*this);
	}

	// Getter
	const std::vector<LocationBlock>	ServerBlock::getLocationBlock() const
	{
		return (this->location_list);
	}

	const std::pair<bool, LocationBlock>	ServerBlock::getLocationBlock(const std::string& request_path) const
	{
		LocationBlock					return_location;
		std::vector<LocationBlock>::const_iterator	current_location = location_list.begin();
		std::vector<LocationBlock>::const_iterator	end_location = location_list.end();

		for (; current_location != end_location; ++current_location)
		{
			if (current_location->getUriPath().length() <= request_path.length()) 
			{
				if (request_path.compare(0, current_location->getUriPath().length(), current_location->getUriPath()) == 0)
					return (std::make_pair(true, *current_location));
			}
		}
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

	void				ServerBlock::setServerName(const std::vector<std::string>& x)
	{
		this->server_name_ = x;
	}

	void				ServerBlock::setServerName(const std::string& x)
	{
		this->server_name_.push_back(x);
	}
	void				ServerBlock::setReturn(const std::vector<std::string>& x)
	{
		this->return_ = x;
	}

	void				ServerBlock::setReturn(const std::string& x)
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

	bool				ServerBlock::checkLocationUriPath(const std::string& uri_path) const
	{
		std::vector<LocationBlock>::const_iterator	current_location = location_list.begin();
		std::vector<LocationBlock>::const_iterator	end_location = location_list.end();

		for (; current_location != end_location; ++current_location)
		{
			if (current_location->getUriPath().compare(uri_path) == 0)
				return (false);
		}
		return (true);
	}

	bool				ServerBlock::checkServerName(const std::string& request_server_name) const
	{
		std::vector<std::string>::const_iterator	current_string = this->server_name_.begin();
		std::vector<std::string>::const_iterator	end_string = this->server_name_.end();

		for (; current_string != end_string; ++current_string)
			if (current_string->compare(request_server_name) == 0)
				return (true);
		return (false);
	}
}
