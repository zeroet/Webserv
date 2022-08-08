#include <iostream>
#include <string>
#include <vector>
#include "HttpBlock.hpp"
#include "PrintConfig.hpp"

int main(int argc, char** argv)
{
	std::pair<bool, ft::HttpBlock>	http_pair;
	ft::HttpBlock	http_context;

	(void)ft::sTokenTypeStrings;
	(void)argc;
	http_pair = ft::PrintConfig::parseConfig(argv[1], P_SERVER); // possible flags: P_TOKEN, P_CONTENT, P_SERVER, P_LOCATION
	if (http_pair.first == true)
	{
		http_context = http_pair.second;
		std::vector<ft::ServerBlock>	server_vector = http_context.getServerBlock();
		ft::ServerBlock			server_context = server_vector.at(0);

		try {
			std::cout << "server name: " << server_context.getServerName(0) << std::endl;
		}
		catch (const std::out_of_range& e) {
			std::cout << "Error: Out of Range.\n";
		}

	}
	return (0);
}
