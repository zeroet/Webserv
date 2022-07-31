#include "connection.hpp"

Connection::Connection(int fd, Block block, Epoll *ep) : clntFd_(fd), block_(block), ep_(ep) {
    std::cout << ep_->getepollfd() << std::endl;
}

Connection::~Connection() { }

void    Connection::processRequest()
{
    // this->request_.addBuffer();
    int n = recv(this->clntFd_, &buffer, sizeof(buffer), 0); 
	// std::cout << "i from recv :" << n << std::endl;
	if (n == -1)
	{
		printf("Error recv\n");
		//need function to quit
		//return (Error); or return (-1);
	}
	std::cout << "n: " << n << std::endl;
    buf_.insert(buf_.end(), buffer, buffer + n);
	std::cout << "buf_: " << std::endl;
	for (unsigned int i = 0; i < buf_.size(); i++)
	{
        // std::cout << "buf size: " << buf_.size() << std::endl;
		std::cout << buf_[i];
	}
    request_.printRequestMsg();
    // this->request_.parsingBuffer();
    // std::cout << this->request_.getBuffer();
    // std::cout << "Recv" << std::endl;
    // ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
    
}

void    Connection::response()
{
    std::cout <<"Response " <<std::endl;
}