#include "HttpBlock.hpp"

namespace ft
{
	HttpBlock::HttpBlock()
	{
	}

	HttpBlock::HttpBlock(const HttpBlock& other) : server_list(other.getServerBlock())
	{
		this->setRoot(other.getRoot());
		this->setClientMaxBodySize(other.getClientMaxBodySize());
		this->setKeepaliveTimeout(other.getKeepaliveTimeout());
		this->setIndex(other.getIndex());
		this->setAutoindex(other.getAutoindex());
		this->setErrorPage(other.getErrorPage());
	}

	HttpBlock::~HttpBlock()
	{
	}

	HttpBlock& HttpBlock::operator= (const HttpBlock& other)
	{
		if (this != &other)
		{
			this->setRoot(other.getRoot());
			this->setClientMaxBodySize(other.getClientMaxBodySize());
			this->setKeepaliveTimeout(other.getKeepaliveTimeout());
			this->setIndex(other.getIndex());
			this->setAutoindex(other.getAutoindex());
			this->setErrorPage(other.getErrorPage());
			this->server_list = other.getServerBlock();
		}
		return (*this);
	}

	std::vector<ServerBlock>	HttpBlock::getServerBlock() const
	{
		return (this->server_list);
	}
};
