#include "epoll.hpp"

class Epoll;

class Request
{
private:
    int fd;
    char buf[512];
    Block block_;
    std::string str_;
    std::string status_;
    int i_;

public:
    Request() {std::cout << "in the map" << std::endl;};
    Request(int epollFd, const int &other, Block block) : fd(other), block_(block), str_("")
    {
        this->status_ =  "connect";
    };

    Request(int num) : fd(num) {
        std::cout << "[" << this->fd << "] client socket number!" <<std::endl;
    };
    void    treat_request(Epoll *ep);

    int    send_string(Epoll *ep);

    ~Request() {};

    std::string getter_status(void)
    {
        return this->status_;
    }

};

void    Request::treat_request(Epoll *ep)
{
        memset(&buf, 0, sizeof(buf));
        i_ = recv(fd, &buf, sizeof(buf), 0); 
        str_ += buf;
        std::cout << "in the recv" << std::endl;
        // ep->epoll_Ctl_Mode(fd, EPOLLOUT);
}

int    Request::send_string(Epoll *ep)
{
        std::cout << "in the send" << std::endl;
        send(fd, str_.c_str(), str_.size(), 0);
        // ep->epoll_Ctl_Mode(fd, EPOLLIN);
        return 0;
}