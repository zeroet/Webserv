#include "connection.hpp"

Connection::Connection(int fd, Block block, Epoll *ep) : clntFd_(fd), block_(block), ep_(ep), request_(fd)
{
    std::cout << ep_->getepollfd() << std::endl;
}

Connection::~Connection() { }

void    Connection::requestRecv()
{
    this->request_.addBuffer();
    this->request_.parsingBuffer();
    // std::cout << this->request_.getBuffer();
    // std::cout << "Recv" << std::endl;
    // ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
    
}

void    Connection::response()
{
    std::cout <<"Response " <<std::endl;
}