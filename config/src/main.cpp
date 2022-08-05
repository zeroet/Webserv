#include <iostream>
#include <string>
#include <vector>
#include "HttpBlock.hpp"
#include "PrintConfig.hpp"

int main(int argc, char** argv)
{
	std::pair<bool, ft::HttpBlock>	httpBlockPair;
	ft::HttpBlock	httpContext;

	(void)ft::sTokenTypeStrings;
	(void)argc;
	httpBlockPair = ft::PrintConfig::parseConfig(argv[1], P_SERVER); // possible flags: P_TOKEN, P_CONTENT, P_SERVER, P_LOCATION
	if (httpBlockPair.first == true)
	{
		httpContext = httpBlockPair.second;
		std::vector<ft::ServerBlock>	serverVector = httpContext.getServerBlock();
		ft::ServerBlock			serverContext = serverVector.at(0);

		try {
			std::cout << "server name: " << serverContext.getServerName(0) << std::endl;
		}
		catch (const std::out_of_range& e) {
			std::cout << "Error: Out of Range.\n";
		}

	}
	return (0);
}
