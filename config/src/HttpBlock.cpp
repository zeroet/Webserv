#include "HttpBlock.hpp"

namespace ft
{
	HttpBlock::HttpBlock()
	{
	}

	HttpBlock::HttpBlock(const HttpBlock& other) : serverList(other.getServerBlock())
	{
	}

	HttpBlock::~HttpBlock()
	{
	}

	HttpBlock& HttpBlock::operator= (const HttpBlock& other)
	{
		if (this != &other)
			serverList = other.getServerBlock();
		return (*this);
	}

	std::vector<ServerBlock>	HttpBlock::getServerBlock() const
	{
		return (this->serverList);
	}
};
