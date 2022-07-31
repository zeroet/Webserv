#include "socket.hpp"

//
//Canonical Form
//

//Construct 
Socket::Socket(void) { }

//Construct for excute
Socket::Socket(std::vector<Block> block) : vecBloc_(block), error_(0)
{
    this->error_ = create_socket();
    if (error_ == ERROR)
        std::cout << "Message : socket create error" << std::endl;
}

//Copy Construct
Socket::Socket(const Socket &other) : vecBloc_(other.vecBloc_)
{
    *this = other;
}

//Destruct
Socket::~Socket(void) {}

//
// Create Socket fd
//if socketFd status is "listen" -> save server fd in vecBloc_
//
int     Socket::create_socket()
{
    int     blockCount = vecBloc_.size();
    struct  sockaddr_in sockAddr;

    for (int i = 0; i < blockCount; i++)
    {
        int socketFd = DEFAULT;

        // socket create
        socketFd = socket(PF_INET, SOCK_STREAM, 0);
        if (socketFd < 0)
			return (ERROR);
		if ((socketFd = socket_nonBlock_setting(socketFd)) < 0)
            return (ERROR);
        if ((socketFd = socket_reUse_setting(socketFd)) < 0)
            return (ERROR);

        // socket struct
        memset(&sockAddr, 0, sizeof(sockAddr));
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(vecBloc_[i].getter_portNumber());
        sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        vecBloc_[i].setter_socketaddr(sockAddr);

        // socket bind && listen
        this->error_ = bind(socketFd, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
        if (this->error_ < 0)
		{
			std::cout << RED << "Bind error ! PortNumber [" << vecBloc_[i].getter_portNumber() <<  "]"<< FIN << std::endl;
			continue;
		}
        this->error_ = listen(socketFd, LISTEN_BACKLOG);
        if (this->error_ < 0)
            return (ERROR);
        if (socketFd < 0)
            return (ERROR);
        else
            vecBloc_[i].setter_socketFd(socketFd);
    }
    return (OK);
}

// socket option setting
// NONBLOCK
// REUSE

int     &Socket::socket_nonBlock_setting(int &socketFd)
{
    int flags = fcntl(socketFd, F_GETFL);

    flags |= O_NONBLOCK;
    if (fcntl(socketFd, F_SETFL, flags) < 0)
        exit(1);
    return (socketFd);
}

int     &Socket::socket_reUse_setting(int &socketFd)
{
    int option = true;
    int error = 0;

    error = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (error < 0)
        exit(1);
    return (socketFd);
}

// getter && setter

Socket::vecBloc Socket::getter_vecBloc() const
{
    return this->vecBloc_;
}
