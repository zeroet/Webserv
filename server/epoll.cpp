#include "epoll.hpp"

Epoll::Epoll() {}

Epoll::Epoll(std::vector<Block> block) : vecBloc_(block)
{
    init_server_socket();
    epoll_server_wait();
}

Epoll::Epoll(const Epoll &other) : vecBloc_(other.vecBloc_), mapClnt_(other.mapClnt_), epollFd_(other.epollFd_)
{
    *this = other;
}

Epoll::~Epoll() { }

void    Epoll::init_server_socket()
{
    int numServerFd = vecBloc_.size();

    create_epoll_fd();
    for (int i = 0; i < numServerFd; i++)
    {
        if (OK != (epoll_add(vecBloc_[i].getter_socketFd())))
            std::cout << "epoll add server socket failed" << std::endl;
    }   
}

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

int    Epoll::epoll_add(int fd)
{
    
    event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    if (epoll_ctl(this->epollFd_, EPOLL_CTL_ADD, fd, &ev) < 0)
    { 
        close(fd);
        return (ERROR);
    }
    return (OK);
}

void    Epoll::epoll_server_wait()
{
    int                 evCount;
    event               epEvent[MAX_EVENT];
    std::pair<int,int>  clnt_serv_pair;
    
    while (1)
    {
        evCount = epoll_wait(this->epollFd_, epEvent, MAX_EVENT, TIMEOUT);
        std::cout << "Epoll event count [ " << evCount <<" ]" << std::endl;
        if (evCount < 0)
            std::cout << "Epoll event count error" << std::endl;
        for (int i = 0; i < evCount; i++)
        {
            clnt_serv_pair = find_server_socket(epEvent[i].data.fd);
            if (clnt_serv_pair.first == ERROR)
                std::cout << "Server socket Not Fount" << std::endl;
            if ((epoll_add(clnt_serv_pair.first)) == ERROR)
                continue ;
        }
    }
}

std::pair<int,int>   Epoll::find_server_socket(int &fd)
{
    int                     size = this->vecBloc_.size();
    struct socketaddr_in    clnt_addr;
    int                     clntFd;
    int                     clntLen = sizeof(clnt_addr);
    std::pair<int,int>      ret;

    for (int i = 0; i < size; i++)
    {
        if (fd == this->vecBloc_[i].getter_socketFd())
        {
            clntFd = accept(this->vecBloc_[i].getter_socketFd(), 
            (struct sockaddr*)&clnt_addr, (socklen_t *)&clntLen);
            if (0 > (clntFd = sock.socket_nonBlock_setting(clntFd)))
                std::cout << "accept() error" << std::endl;
            else
            { 
                ret.first = clntFd;
                ret.second = this->vecBloc_[i].getter_socketFd();
                return (ret);
            }
        }
    }
    ret.first = fd;
    ret.second = -1;
    return (ret);

}

void    Epoll::close_all_serv_socket()
{
    int count = this->vecBloc_.size();
    
    for(int i = 0; i < count; i++)
        close(this->vecBloc_[i].getter_socketFd());
    close(this->epollFd_);
    std::cout << "all socket closed" << std::endl;
}