#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include <streambuf>
// #include "R_server/epoll.hpp"
#include "Request.hpp"

void print_map(std::map<std::string, std::string>& m) {
    for (std::map<std::string, std::string>::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        std::cout << itr->first << " " << itr->second << std::endl;
    }
}

// std::ostream & safegetline( std::ostream & os, std::string & line ) {
//     std::string myline;
//     if ( std::getline( os, myline ) ) {
//        if ( myline.size() && myline[myline.size()-1] == '\r' ) {
//            line = myline.substr( 0, myline.size() - 1 );
//        }
//        else {
//            line = myline;
//        }
//     }
//     return os;
// }

int	main(int ac, char** av) {

	if (ac == 2)
	{
		std::ifstream	file(av[1]);
		std::string		line;
		char			c;
		// int				flag = 0;
		// int i = 0;
		while(std::getline(file, line, '\r'))
		{
			std::cout << line;
			// if (flag == 1 && ((c=file.get())!='\n') file.rdbuf()->sputbackc(c))
				// std::cout << "EOF" << std::endl;
			// else
				// flag = 0;
			if ((c=file.get())!='\n')
			{
				file.rdbuf()->sputbackc(c);
			// if (line.find("\r\n") != std::string::npos)
				// flag = 1;
				std::cout << "IN" << std::endl;
			}

		}
	}
	return (0);

	// std::vector<Block> server;
    // Block a(8080);
    // Block b(8000);
    // Block c(7000);
    // server.push_back(a);
    // server.push_back(b);
    // server.push_back(c);

    // Socket socket(server);
    // std::vector<Block> test = socket.getter_vecBloc();

    // std::cout << "Server Fd : " << test[0].getter_socketFd() << std::endl;
    // std::cout << "Server Port : " << test[0].getter_portNumber() << std::endl;

    // std::cout << "Server Fd : " << test[1].getter_socketFd() << std::endl;
    // std::cout << "Server Port : " << test[1].getter_portNumber() << std::endl;

    // std::cout << "Server Fd : " << test[2].getter_socketFd() << std::endl;
    // std::cout << "Server Port : " << test[2].getter_portNumber() << std::endl;

	// Epoll epoll(test);
	// if (ac != 2)
	// {
	// 	std::cout << "parse exemple needed" << std::endl;
	// 	return (1);
	// }
	// else
	// {


	// }

    // Request request;

    // request.POST2Req(&request);

    // std::cout << request.getMethod() << std::endl;
    // std::cout << request.getPath() << std::endl;
    // std::cout << request.getVersion() << std::endl;
    // print_map(request.getRequestHeader());
    // std::cout << request.getRequestBody() << std::endl;
}
