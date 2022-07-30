#pragma once

#include <iostream>
#include "socket.hpp"
// #include "connection.hpp"
#include <sys/epoll.h>
#include <map>

# define CRLF "\r\n"
# define END_BUF "\r\n\r\n"
# define LEN_END_BUF 4
# define LEN_CRLF 2

class Request
{
private:
    std::string buf_;
    char buf[512];
    int i_;
    int fd_;

public:
    Request();
    Request(int fd);
    Request(Request const &other);
    ~Request();

    void    addBuffer(void);
    void    parsingBuffer(void);
    //getter
    std::string const &getBuffer(void) const;
};