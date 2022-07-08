#include "epoll.hpp"

Epoll::Epoll() {}

Epoll::Epoll(std::vector<Block> block) : vecBloc_(block)
{
    create_epoll_fd();
}

Epoll::Epoll(const Epoll &other) : vecBloc_(other.vecBloc_), mapClnt_(other.mapClnt_), epollFd_(other.epollFd_)
{
    *this = other;
}

Epoll::~Epoll() { }

void    Epoll::create_epoll_fd()
{
    this->epollFd_ = epoll_create(MAX_EVENT);
    if (epollFd_ < 0)
    { 
        close_all_serv_socket();
        // tmp exit error
        std::cout << "Epoll fail" << std::endl;
        exit(1);
    }
}

void    Epoll::close_all_serv_socket()
{
    int count = vecBloc_.size();
    
    for(int i = 0; i < count; i++)
        close(vecBloc_[i].getter_socketFd());
    close(this->epollFd_);
    std::cout << "all socket closed" << std::endl;
}