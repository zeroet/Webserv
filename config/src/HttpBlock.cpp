#include "HttpBlock.hpp"

namespace ft
{
	const std::vector<ServerBlock>	HttpBlock::getServerBlock() const
	{
		return (this->serverList);
	}
};
