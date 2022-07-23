// #include "socket.hpp"
#include "epoll.hpp"
// #include "requestTest.hpp"

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
	Epoll epoll(test);



    return (0);
}
