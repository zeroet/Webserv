#ifndef HTTPBLOCK_HPP
#define HTTPBLOCK_HPP

#include <vector>
#include <string>
#include "BaseDirectives.hpp"
//#include "Directive.hpp"
#include "ServerBlock.hpp"
#include <bits/stdc++.h>

namespace ft
{
	class ServerBlock;
	class HttpBlock : public BaseDirectives
	{
		public:
			std::vector<ServerBlock>	serverList;
		public:
			//HttpBlock();
			std::vector<ServerBlock>	getServerBlock() const;
	};

}

#endif
