//#include "socket.hpp"
#include "epoll.hpp"

int main()
{
    std::vector<Block> server;
    Block a(10);
    Block b(8010);
    Block c(9500);
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

	Epoll epoll(test);



    return (0);
}
