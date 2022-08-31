#include "BaseDirectives.hpp"

namespace ft
{
	BaseDirectives::BaseDirectives()
	{
		this->root_ = "html";
		this->client_max_body_size_ = 1000000;
		this->keepalive_timeout_ = 75;
		this->index_.push_back("index.html");
		this->autoindex_ = false;
		this->error_page_.push_back("");
		this->config_path_ = DEFAULT_CONF;
	}

	// Getter	
	const std::string		BaseDirectives::getRoot(void) const
	{
		return (this->root_);
	}

	unsigned long		BaseDirectives::getClientMaxBodySize(void) const
	{
		return (this->client_max_body_size_);
	}

	unsigned int		BaseDirectives::getKeepaliveTimeout(void) const
	{
		return (this->keepalive_timeout_);
	}

	const std::vector<std::string>	BaseDirectives::getIndex(void) const
	{
		return (this->index_);
	}

	bool			BaseDirectives::getAutoindex(void) const
	{
		return (this->autoindex_);
	}

	const std::vector<std::string>		BaseDirectives::getErrorPage(void) const
	{
		return (this->error_page_);
	}

	const std::string		BaseDirectives::getErrorPage(size_t	index) const
	{
		return (this->error_page_.at(index));
	}

	const std::string		BaseDirectives::getErrorPagePath(void) const
	{
		return (*(--this->error_page_.end()));
	}

	const std::vector<std::string>		BaseDirectives::getErrorPageCode(void) const
	{
		std::vector<std::string>		code_vector;	
		std::vector<std::string>::const_iterator	current_string = error_page_.begin();
		std::vector<std::string>::const_iterator	end_string = error_page_.end();

		for (; current_string != end_string - 1; ++current_string)
			code_vector.push_back(*current_string);
		return (code_vector);
	}

	const std::string		BaseDirectives::getConfigPath(void) const
	{
		return (this->config_path_);
	}

	// Setter
	void				BaseDirectives::setRoot(const std::string x)
	{
		this->root_ = x;
	}

	void				BaseDirectives::setClientMaxBodySize(const unsigned long x)
	{
		this->client_max_body_size_ = x;
	}

	void				BaseDirectives::setKeepaliveTimeout(const unsigned int x)
	{
		this->keepalive_timeout_ = x;
	}

	void				BaseDirectives::setIndex(const std::vector<std::string> x)
	{
		this->index_ = x;
	}

	void				BaseDirectives::setIndex(const std::string x)
	{
		this->index_.push_back(x);
	}

	void				BaseDirectives::setAutoindex(const bool x)
	{
		this->autoindex_ = x;
	}

	void				BaseDirectives::setErrorPage(const std::vector<std::string> x)
	{
		this->error_page_ = x;
	}

	void				BaseDirectives::setErrorPage(const std::string x)
	{
		this->error_page_.push_back(x);
	}

	void				BaseDirectives::setConfigPath(const std::string x)
	{
		this->config_path_ = x;
	}

	void				BaseDirectives::clearIndex(void)
	{
		this->index_.clear();
	}

	void				BaseDirectives::clearErrorPage(void)
	{
		this->error_page_.clear();
	}
}
