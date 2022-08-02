#include "BaseDirectives.hpp"

namespace ft
{
	BaseDirectives::BaseDirectives()
	{
		this->root_ = "html";
		this->clientMaxBodySize_ = 1000000;
		this->keepaliveTimeout_ = 75;
		this->index_.push_back("index.html");
		this->autoindex_ = "off";
		this->errorPage_ = "";
	}

	// Getter	
	const std::string		BaseDirectives::getRoot(void) const
	{
		return (this->root_);
	}

	unsigned long		BaseDirectives::getClientMaxBodySize(void) const
	{
		return (this->clientMaxBodySize_);
	}

	unsigned int		BaseDirectives::getKeepaliveTimeout(void) const
	{
		return (this->keepaliveTimeout_);
	}

	const std::vector<std::string>	BaseDirectives::getIndex(void) const
	{
		return (this->index_);
	}

	bool			BaseDirectives::getAutoindex(void) const
	{
		return (this->autoindex_);
	}

	const std::string		BaseDirectives::getErrorPage(void) const
	{
		return (this->errorPage_);
	}

	// Setter
	void				BaseDirectives::setRoot(const std::string x)
	{
		this->root_ = x;
	}

	void				BaseDirectives::setClientMaxBodySize(const unsigned long x)
	{
		this->clientMaxBodySize_ = x;
	}

	void				BaseDirectives::setKeepaliveTimeout(const unsigned int x)
	{
		this->keepaliveTimeout_ = x;
	}

	void				BaseDirectives::setIndex(const std::string x)
	{
		this->index_.push_back(x);
	}

	void				BaseDirectives::setAutoindex(const bool x)
	{
		this->autoindex_ = x;
	}

	void				BaseDirectives::setErrorPage(const std::string x)
	{
		this->errorPage_ = x;
	}


	void				BaseDirectives::clearIndex(void)
	{
		this->index_.clear();
	}

}
