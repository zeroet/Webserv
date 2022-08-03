#include "connection.hpp"

Connection::Connection(int fd, ServerBlock block, Epoll *ep) : clntFd_(fd), block_(block), ep_(ep)
{
}

Connection::~Connection() { }

void    Connection::requestRecv()
{
    char str[1024] = "";

    std::cout << "Recv" << std::endl;
    if (0 < recv(clntFd_, &str, 1024, 0))
    {
        std::cout << block_.getRoot() << std::endl;
        std::cout << block_.getSocketFd() << std::endl;
        std::cout << block_.getKeepaliveTimeout() << std::endl;
        std::cout << this->clntFd_ << std::endl;
        
        ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
    }
}

void    Connection::response()
{
    std::cout <<"Response " <<std::endl;
    ep_->epoll_Ctl_Mode(this->clntFd_, EPOLLIN);
}