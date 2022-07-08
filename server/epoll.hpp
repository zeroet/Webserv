#include "socket.hpp"
#include <sys/epoll.h>
#include <map>

# define MAX_EVENT 1024
# define TIMEOUT -1

class Request
{
    Request();
};

class Epoll
{

public:
    typedef int                         clntFd;
    typedef std::vector<Block>          vecBloc;
    typedef std::map<clntFd, Request>   mapClnt;
    typedef struct epoll_event          event;

private:
    vecBloc vecBloc_;
    mapClnt mapClnt_;
    event   ev_[MAX_EVENT];
    int     epollFd_;

public:
    Epoll();
    Epoll(std::vector<Block> block); // epoll fd create
    Epoll(const Epoll &other);
    ~Epoll();
};
