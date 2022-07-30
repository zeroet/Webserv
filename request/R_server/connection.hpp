#pragma once

#include "epoll.hpp"
#include "request.hpp"

//class Request;
class Epoll;
class Request;

class Connection
{
private:
    int clntFd_;
    Block   block_;
    std::string status_;
    Epoll *ep_;
    Request request_;
    // res
    // exe
public:
    Connection(int fd, Block block, Epoll *ep);
    ~Connection();

    void    requestRecv();
    void    response();

};