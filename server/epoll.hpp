#include "socket.hpp"
#include <sys/epoll.h>
#include <map>

# define MAX_EVENT 1024
# define TIMEOUT -1

class Request
{
    Request() {std::cout <<" hi im request!" <<std::endl;};
};

class Epoll
{

public:
    typedef int                         clntFd;
    typedef std::vector<Block>          vecBloc;
    typedef std::map<clntFd, Request>   mapClnt;
    typedef struct epoll_event          event;
    Socket                              sock;

private:
    vecBloc vecBloc_;
    mapClnt mapClnt_;
    int     epollFd_;

public:
    Epoll();
    Epoll(std::vector<Block> block); // epoll fd create
    Epoll(const Epoll &other);
    ~Epoll();

    void            init_server_socket();
    void            create_epoll_fd();
    int             epoll_add(int fd);
    void            epoll_server_manager();
    void            close_all_serv_socket();

    int                 find_server_fd(int fd);
    std::pair<int,int>  create_clnt_socket(int &fd);
};
