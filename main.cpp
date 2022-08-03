#include <iostream>
#include <string>
#include <vector>
#include "config/src/HttpBlock.hpp"
#include "config/src/PrintConfig.hpp"

#include "server/epoll.hpp"

int main(int argc, char** argv)
{
	std::pair<bool, ft::HttpBlock>	httpContext;

	(void)ft::sTokenTypeStrings;
	(void)argc;
	httpContext = ft::PrintConfig::parseConfig(argv[1], P_SERVER); // possible flags: P_TOKEN, P_CONTENT, P_SERVER, P_LOCATION
	//bool error if ()
	Socket socket(httpContext.second.getServerBlock());
	Epoll epoll(socket.getter_vecBloc());

	return (0);
}
