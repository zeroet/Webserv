#ifndef HTTPBLOCK_HPP
#define HTTPBLOCK_HPP

#include "BaseDirectives.hpp"
#include "ServerBlock.hpp"

namespace ft
{
	class ServerBlock;
	class HttpBlock : public BaseDirectives
	{
		public:
			std::vector<ServerBlock>	serverList;
		public:
			HttpBlock();
			HttpBlock(const HttpBlock& other);
			~HttpBlock();
			HttpBlock& operator= (const HttpBlock& other);
			std::vector<ServerBlock>	getServerBlock() const;
	};

}

#endif
