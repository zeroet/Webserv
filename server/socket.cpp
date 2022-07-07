#include "socket.hpp"

Socket::Socket(void) { }

Socket::Socket(const std::vector<Block> &block) : vecBloc_(block), error_(0)
{
    this->error_ = create_socket();
    if (error_ == ERROR)
    { 
        std::cout << "Message : socket create error" << std::endl;
        close(socketFd);
    }
}

Socket::Socket(const Socket &other) : vecBloc_(other.vecBloc_)
{
    *this = other;
    return *this;
}

Socket::~Socket(void) {}

int     Socket::create_socket()
{
    int     blockCount = 4;
    int     socketFd = 0;
    struct  socketaddr_in sockAddr;

    for (int i = 0; i < blockCount; i++)
    {
        socketFd = socket(PF_INET, SOCK_STREAM, 0);
        if (error < 0)
            return (ERROR);
        socketFd = serverSocket_setting(socketFd);
        memset(&sockAddr, 0, sizeof(sockAddr));
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(vecBlock_[i].getter_portNumber());
        sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        vecBlock_[i].setter_socketaddr(sockAddr);

        this->error_ = bind(socketFd, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
        if (this->error_ < 0)
            return (ERROR);
        this-error_ = listen(socketFd, LISTEN_BACKLONG);
        if (this->error_ < 0)
            return (ERROR);
        vecServ_[i].setter_serverFd(socketFd);
    }
}

int     Socket::serverSocket_setting(int socketFd)
{
    int flags = fcntl(socketFd, F_GETFL);
    int option = true;
    int error = 0;

    flags |= O_NONBLOCK;
    if (fcntl(socketFd, F_SETFL, flags) < 0)
        return (ERROR);
    error = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (error < 0)
        return (ERROR);
    return (OK);
}

vecBloc Socket::getter_vecBloc() const
{
    return this->vecBloc_;
}