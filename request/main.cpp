#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include "../server/epoll.hpp"
// #include "../server/socket.hpp"
#include "Request.hpp"

int	main(int ac, char** av) {

	(void)av;
	(void)ac;

	std::vector<Block> server;
    Block a(8080);
    Block b(8000);
    Block c(7000);
    server.push_back(a);
    server.push_back(b);
    server.push_back(c);

    Socket socket(server);
    std::vector<Block> test = socket.getter_vecBloc();

    std::cout << "Server Fd : " << test[0].getter_socketFd() << std::endl;
    std::cout << "Server Port : " << test[0].getter_portNumber() << std::endl;

    std::cout << "Server Fd : " << test[1].getter_socketFd() << std::endl;
    std::cout << "Server Port : " << test[1].getter_portNumber() << std::endl;

    std::cout << "Server Fd : " << test[2].getter_socketFd() << std::endl;
    std::cout << "Server Port : " << test[2].getter_portNumber() << std::endl;

	// if (ac != 2)
	// {
	// 	std::cout << "parse exemple needed" << std::endl;
	// 	return (1);
	// }
	// else
	// {


	// }
	return (0);
}
