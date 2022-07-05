#include "socket.hpp"

Socket::Socket(void) {}

Socket::Socket(const Socket &other) : socketFd_(other.socketFd_)
{
    *this = other;
}

Socket::~Socket(void) {}


// Socket create && non blocking socket setting 
// only non-blocking socket needed for this project
for (block.size)
{
    block size 만큼 create server socket && std::vector<int> vecServ; fd 저장
}

int     Socket::create_socket()
{
    this->socketFd_ = socket(PF_INET, SOCK_STREAM, 0);
    if (this->socketFd_ < 0)
        return (ERROR);
 
    //server fd Non-Blocking Socket 으로 설정. Edge-Trigger사용 위해
    int flags = fcntl(this->socketFd_, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(this->socketFd_, F_SETFL, flags) < 0)
        return (ERROR);

    // Option -> SO_REUSEADDR : 비정상종료 포트 재사용
    int option = true;
    this->error_ = setsockopt(this->socketFd_, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (this->error_ < 0)
    { 
        close(this->socketFd_);
        return (ERROR);
    }
    std::cout << "Create socket [" << this->socketFd_ <<"] " << std::endl; 
    return (OK);
}


// getter && setter 
void    Socket::setSocketFd(int fd)
{

}

int     Socket::getSocketFd(void)
{ 

}