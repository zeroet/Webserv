#pragma once

#include "epoll.hpp"
#include "../config/src/ServerBlock.hpp"

namespace ft{


class Epoll;

class Connection
{
private:
    int clntFd_;
    ServerBlock   block_;
    std::string status_;
    Epoll *ep_;


public:
    Connection(int fd, ServerBlock block, Epoll *ep);
    ~Connection();

    void    requestRecv();
    void    response();

};

}