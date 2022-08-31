#include <iostream>
#include <string>
#include <vector>
#include "HttpBlock.hpp"
#include "PrintConfig.hpp"
#define	REQUEST_SERVER_NAME "worldhello"
#define LISTEN_PORT 9000

int main(int argc, char** argv)
{
	std::pair<bool, ft::HttpBlock>	http_pair;
	std::pair<bool, ft::ServerBlock>	server_pair;
	ft::HttpBlock	http_context;

	(void)ft::sTokenTypeStrings;
	(void)argc;
	http_pair = ft::PrintConfig::parseConfig(argv[1], P_LOCATION); // possible flags: P_TOKEN, P_CONTENT, P_SERVER, P_LOCATION
	if (http_pair.first == true)
	{
		http_context = http_pair.second;
		std::vector<ft::ServerBlock>		server_vector = http_context.getServerBlock();
		ft::ServerBlock				server_context = server_vector.at(0);

		try {
			//std::cout << "server name: " << server_context.getServerName(0) << std::endl; // getServerName by index
			std::pair<bool, ft::LocationBlock>  location_pair = server_context.getLocationBlock("/cgi_teste/dfjk"); // getLocationBlock by request_path
			//std::cout << http_pair.second.getErrorPagePath() << std::endl;
			if (location_pair.first == true)
				std::cout << "selected uri: " << location_pair.second.getUriPath() << "\n";

/*
			// listen port, server_name duplication test
			server_pair = http_context.getServerBlock(LISTEN_PORT, REQUEST_SERVER_NAME); // getServerBlock by listen, server_name
			if (server_pair.first == true)
			{
				std::cout << "case 0. listen: " << LISTEN_PORT << ", surver_name: " << REQUEST_SERVER_NAME << "\n";
				std::cout << "result: " << server_pair.second.getListen() << ", ";
				std::cout << server_pair.second.getServerName(0) << "\n";
			}
			server_pair = http_context.getServerBlock(LISTEN_PORT); // getServerBlock by listen, server_name
			if (server_pair.first == true)
			{
				std::cout << "case 1. listen: " << LISTEN_PORT << ", unspecified surver_name \n";
				std::cout << "result: " << server_pair.second.getListen() << ", ";
				std::cout << server_pair.second.getServerName(0) << "\n";
			}
			*/
		}
		catch (const std::out_of_range& e) {
			std::cout << "Error: Out of Range.\n";
		}

	}
	return (0);
}
