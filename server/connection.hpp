#pragma once

#include "epoll.hpp"

class Epoll;

class Connection
{
private:
    int clntFd_;
    Block   block_;
    std::string status_;
    Epoll *ep_;


public:
    Connection(int fd, Block block, Epoll *ep);
    ~Connection();

    void    requestRecv();
    void    response();

};