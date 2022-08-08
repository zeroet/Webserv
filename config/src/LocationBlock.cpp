#include "LocationBlock.hpp"

namespace ft
{
	LocationBlock::LocationBlock()
	{
	}

	// Constructor
	LocationBlock::LocationBlock(const BaseDirectives& context)
	{
		this->root_ = context.getRoot();
		this->clientMaxBodySize_ = context.getClientMaxBodySize();
		this->keepaliveTimeout_ = context.getKeepaliveTimeout();
		this->index_ = context.getIndex();
		this->autoindex_ = context.getAutoindex();
		this->errorPage_ = context.getErrorPage();
	}
	LocationBlock::LocationBlock(const LocationBlock& other)
	{
		if (this != &other)
		{
			this->setLimitExcept(other.getLimitExcept());
			this->setReturn(other.getReturn());
			this->setCgi(other.getCgi());
			this->setCgiPath(other.getCgiPath());
		}
	}

	LocationBlock::~LocationBlock()
	{
	}

	LocationBlock& LocationBlock::operator= (const LocationBlock& other)
	{
		if (this != &other)
		{
			this->setLimitExcept(other.getLimitExcept());
			this->setReturn(other.getReturn());
			this->setCgi(other.getCgi());
			this->setCgiPath(other.getCgiPath());
		}
		return (*this);
	}
	
	// Getter
	const std::vector<std::string>	LocationBlock::getLimitExcept(void) const
	{
		return (this->limitExcept_);
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
		return (this->cgiPath_);
	}

	// Setter
	void				LocationBlock::setLimitExcept(const std::vector<std::string> x)
	{
		this->limitExcept_ = x;
	}

	void				LocationBlock::setLimitExcept(const std::string x)
	{
		this->limitExcept_.push_back(x);
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
		this->cgiPath_ = x;
	}

}
