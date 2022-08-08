#include "LocationBlock.hpp"

namespace ft
{
	// Constructor
	LocationBlock::LocationBlock()
	{
	}

	LocationBlock::LocationBlock(const BaseDirectives& context)
	{
		this->root_ = context.getRoot();
		this->client_max_body_size_ = context.getClientMaxBodySize();
		this->keepalive_timeout_ = context.getKeepaliveTimeout();
		this->index_ = context.getIndex();
		this->autoindex_ = context.getAutoindex();
		this->error_page_ = context.getErrorPage();
	}
	LocationBlock::LocationBlock(const LocationBlock& other)
	{
		if (this != &other)
		{
			this->setRoot(other.getRoot());
			this->setClientMaxBodySize(other.getClientMaxBodySize());
			this->setKeepaliveTimeout(other.getKeepaliveTimeout());
			this->setIndex(other.getIndex());
			this->setAutoindex(other.getAutoindex());
			this->setErrorPage(other.getErrorPage());
			this->setLimitExcept(other.getLimitExcept());
			this->setReturn(other.getReturn());
			this->setCgi(other.getCgi());
			this->setCgiPath(other.getCgiPath());
			this->setUriPath(other.getUriPath());
		}
	}

	LocationBlock::~LocationBlock()
	{
	}

	LocationBlock& LocationBlock::operator= (const LocationBlock& other)
	{
		if (this != &other)
		{
			this->setRoot(other.getRoot());
			this->setClientMaxBodySize(other.getClientMaxBodySize());
			this->setKeepaliveTimeout(other.getKeepaliveTimeout());
			this->setIndex(other.getIndex());
			this->setAutoindex(other.getAutoindex());
			this->setErrorPage(other.getErrorPage());
			this->setLimitExcept(other.getLimitExcept());
			this->setReturn(other.getReturn());
			this->setCgi(other.getCgi());
			this->setCgiPath(other.getCgiPath());
			this->setUriPath(other.getUriPath());
		}
		return (*this);
	}
	
	// Getter
	const std::vector<std::string>	LocationBlock::getLimitExcept(void) const
	{
		return (this->limit_except_);
	}

	const std::vector<std::string>	LocationBlock::getReturn(void) const
	{
		return (this->return_);
	}
	const std::vector<std::string>	LocationBlock::getCgi(void) const
	{
		return (this->cgi_);
	}

	const std::string		LocationBlock::getCgiPath(void) const
	{
		return (this->cgi_path_);
	}

	const std::string		LocationBlock::getUriPath(void) const
	{
		return (this->uri_path_);
	}

	// Setter
	void				LocationBlock::setLimitExcept(const std::vector<std::string> x)
	{
		this->limit_except_ = x;
	}

	void				LocationBlock::setLimitExcept(const std::string x)
	{
		this->limit_except_.push_back(x);
	}

	void				LocationBlock::setReturn(const std::vector<std::string> x)
	{
		this->return_ = x;
	}

	void				LocationBlock::setReturn(const std::string x)
	{
		this->return_.push_back(x);
	}

	void				LocationBlock::setCgi(const std::vector<std::string> x)
	{
		this->cgi_ = x;
	}

	void				LocationBlock::setCgi(const std::string x)
	{
		this->cgi_.push_back(x);
	}

	void				LocationBlock::setCgiPath(const std::string x)
	{
		this->cgi_path_ = x;
	}

	void				LocationBlock::setUriPath(const std::string x)
	{
		this->uri_path_ = x;
	}

}
