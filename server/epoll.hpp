#pragma once

#include "socket.hpp"
#include <sys/epoll.h>
#include <map>

// #include "response/Response.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>



# define MAX_EVENT 1024
# define TIMEOUT -1

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
    Request() {};
    Request(const int &other, Block block) : fd(other), block_(block), str_("")
    {
        this->status_ =  "connect";
        std::cout << YELLOW << "--- Connection info ---" << std::endl;
        std::cout << "Server Fd  : " << block.getter_socketFd() << std::endl;
        std::cout << "Client Fd  : " << this->fd << std::endl;
        std::cout << "Server PortNumber : " << block.getter_portNumber() << std::endl;
        std::cout << "Connection Status : " << this->status_ << std::endl;
        std::cout << FIN << std::endl;
    };

    void    treat_request()
    {
        memset(&buf, 0, sizeof(buf));
        i_ = recv(fd, &buf, sizeof(buf), 0); 
        str_ += buf;
        std::cout << "in the recv" << std::endl;
        // ep->epoll_Ctl_Mode(this->fd, EPOLLOUT);

    }

    int    send_string()
    {
        std::cout << "in the send" << std::endl;
        send(fd, str_.c_str(), str_.size(), 0);
        // ep->epoll_Ctl_Mode(this->fd, EPOLLIN);
        return 0;
    }

    ~Request() {};

    std::string getter_status(void)
    {
        return this->status_;
    }

};


class Epoll
{
public:
    typedef int                         clntFd;
    typedef std::vector<Block>          vecBloc;
    typedef std::map<clntFd, Request>   mapClnt;
    typedef struct epoll_event          event;
    typedef event*                      pEvent;;
    typedef std::pair<int, Request>     mapPair;
    Socket                              sock;

private:
    vecBloc vecBloc_; // vector type location block 
    mapClnt mapClnt_; // map type connection poll 
    int     epollFd_; 

public:
    Epoll();
    Epoll(std::vector<Block> block); // epoll fd create
    Epoll(const Epoll &other);
    ~Epoll();

    //Epoll utility functions
    void            init_server_socket();
    void            create_epoll_fd();
    int             epoll_add(int fd);
    void            epoll_Ctl_Mode(int fd, int op);


    //Epoll Main management function
    void            epoll_server_manager();

    //Utility Functions
    void            close_all_serv_socket();
    int             find_server_fd(int fd);
    int             create_clnt_socket(int fd);

    //Block class or utile ????
    Block           get_location_block(int fd);

};

