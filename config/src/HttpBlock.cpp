#include "HttpBlock.hpp"

namespace ft
{
	HttpBlock::HttpBlock()
	{
	}

	HttpBlock::HttpBlock(const HttpBlock& other) : server_list(other.getServerBlock())
	{
	}

	HttpBlock::~HttpBlock()
	{
	}

	HttpBlock& HttpBlock::operator= (const HttpBlock& other)
	{
		if (this != &other)
			server_list = other.getServerBlock();
		return (*this);
	}

	std::vector<ServerBlock>	HttpBlock::getServerBlock() const
	{
		return (this->server_list);
	}
};
