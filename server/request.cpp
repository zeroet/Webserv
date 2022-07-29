#include "request.hpp"

Request::Request(int fd) : buf_(""), fd_(fd)
{
}

Request::Request()
{

}

Request::Request(Request const &other)
{
    this->buf_ = other.buf_;
    this->fd_ = other.fd_;
}

Request::~Request()
{

}

void    Request::addBuffer(void)
{
    memset(&buf, 0, sizeof(buf));
    i_ = recv(this->fd_, &buf, sizeof(buf), 0); 
    buf_ += buf;
}

void    Request::parsingBuffer(void)
{
    std::string tmp;
    std::string::size_type posEndBuff(-1);
    std::string::size_type posCRLF(-1);

    std::cout << posEndBuff << " est pos End Buff" <<  std::endl;
    /*
    posEndBuff = this->buf_.find(END_BUF);
    posCRLF = this->buf_.find(CRLF);
    // buf_ : "abc/r/n/r/ndef"
    // n = 3

    std::cout << posEndBuff << " est pos End Buff" <<  std::endl;

    if (posEndBuff != -1)
    {
        tmp.assign(this->buf_, 0, posEndBuff);
        this->buf_.erase(0, posEndBuff + LEN_END_BUF);
    }
    std::cout << tmp << " est tmp\n";
    std::cout << buf_ << " est buf\n";
    */
}

std::string const &Request::getBuffer(void) const
{
    return (this->buf_);
}