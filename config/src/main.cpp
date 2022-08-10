#include <iostream>
#include <string>
#include <vector>
#include "HttpBlock.hpp"
#include "PrintConfig.hpp"
#define	REQUEST_SERVER_NAME "alias"

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
			//std::cout << "server name: " << server_context.getServerName(0) << std::endl; // getServerName by index
			std::pair<bool, ft::LocationBlock>  location_pair = server_context.getLocationBlock("/cgi_teste/dfjk"); // getLocationBlock by request_path
			if (location_pair.first == true)
				std::cout << "selected uri: " << location_pair.second.getUriPath() << "\n";
			std::pair<bool, ft::ServerBlock> server_pair = http_context.getServerBlock(9090, REQUEST_SERVER_NAME); // getServerBlock by listen, server_name
			if (server_pair.first == true)
			{
				std::cout << server_pair.second.checkServerName(REQUEST_SERVER_NAME) << "\n";
				std::cout << server_pair.second.getServerName(1) << "\n";
			}
		}
		catch (const std::out_of_range& e) {
			std::cout << "Error: Out of Range.\n";
		}

	}
	return (0);
}
