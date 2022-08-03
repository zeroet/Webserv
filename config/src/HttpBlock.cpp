#include "HttpBlock.hpp"

namespace ft
{
	std::vector<ServerBlock>	HttpBlock::getServerBlock() const
	{
		return (this->serverList);
	}
};
