#ifndef EPOLL_HPP
# define EPOLL_HPP

#include "socket.hpp"
#include "connection.hpp"
#include <sys/epoll.h>
#include <map>

// #include "response/Response.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
//#include "../config/src/serverBlock.hpp"


# define MAX_EVENT 1024
# define TIMEOUT -1


class Connection;
class Request;

class Epoll
{
public:
    typedef int                             clntFd;
    typedef std::vector<ft::ServerBlock>        vecBloc;
    typedef std::map< clntFd, Connection >    mapConnection;
    typedef struct epoll_event              event;
    typedef event*                          pEvent;;
    typedef std::pair< int, Connection >      mapPair;
    Socket                              sock;

private:
    vecBloc         vecBloc_; // vector type location block
    mapConnection   c_; // map type connection poll
    int             epollFd_;

public:
    Epoll();
    Epoll(std::vector<ft::ServerBlock> block); // epoll fd create
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
//et nu
    ft::ServerBlock           get_location_block(int fd);
    int             getepollfd()
    {
        return (epollFd_);
    }
};

#endif
