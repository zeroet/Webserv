#include "socket.hpp"
#include <sys/epoll.h>
#include <map>

// #include "request.hpp"

class Epoll
{
public:
    typedef std::vector<Block>      vecBloc;
    typedef std::map<int, Block>    mapClnt; // key : clnt socket number, value : Block class for socket && config data
    
    Epoll();
    Epoll(std::vector<Block> block);
    Epoll(const Epoll &other);
    ~Epoll();

private:
    vecBloc vecBloc_;
    mapClnt mapClnt_;




}
