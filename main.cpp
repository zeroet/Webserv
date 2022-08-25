#include <iostream>
#include <string>
#include <vector>
#include "config/src/HttpBlock.hpp"
#include "config/src/PrintConfig.hpp"
#include "server/epoll.hpp"
// #include "server/socket.hpp"
// #include "request/request.hpp"
// #include "server/connection.hpp"

int main(int argc, char** argv)
{
	char	filePath[] = "config/conf/sample.conf";
	std::pair<bool, ft::HttpBlock>	httpContext;

	(void)ft::sTokenTypeStrings;
	if (argc == 1) 
		httpContext = ft::PrintConfig::parseConfig(filePath, P_SERVER); // possible flags: P_TOKEN, P_CONTENT, P_SERVER, P_LOCATION
	else
		httpContext = ft::PrintConfig::parseConfig(argv[1], P_SERVER); // possible flags: P_TOKEN, P_CONTENT, P_SERVER, P_LOCATION
	//bool error if ()
	if (httpContext.first == 1)
	{
			Socket socket(httpContext.second.getServerBlock());
			Epoll epoll(socket.getter_vecBloc());
	}
	std::cerr << "Config error" << std::endl;
	return (0);
}
