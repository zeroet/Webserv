#include "connection.hpp"

Connection::Connection(int fd, Block block, Epoll *ep) : clntFd_(fd), block_(block), ep_(ep), Ctl_mode_flag_(false) {
    std::cout << ep_->getepollfd() << std::endl;
}

Connection::~Connection() { }

void    Connection::processRequest()
{
    // this->request_.addBuffer();
    int n = recv(this->clntFd_, &buffer_char, sizeof(buffer_char) - 1, 0); //except \r
	// std::cout << "i from recv :" << n << std::endl;
	if (n == -1)
	{
		printf("recv Error\n");
		//close connection
		return ;
		//need function to quit
		//return (Error); or return (-1);
	}
	std::cout << "n: " << n << std::endl;
	std::cout << "buffer_char: " << buffer_char << std::endl;
	// std::cout << "buffer_char + n: " << buffer_char + n << std::endl;
    // buf_.insert(buf_.end(), buffer_char, buffer_char + n);
	// std::string a(buffer_char);
	// tmp.append(a);
	buffer_.insert(buffer_.end(), buffer_char, buffer_char + n);
	std::cout << "buffer_: " << buffer_ << std::endl;

	size_t pos;
	if ((pos = buffer_.find(CRLFCRLF)) != std::string::npos)
			setCtlMode(true);



	// for (unsigned int i = 0; i < buf_.size(); i++)
	// {
    //     // std::cout << "buf size: " << buf_.size() << std::endl;
	// 	std::cout << buf_[i];
	// }
	// if (std::find(buf_.begin(), buf_.end(), '\r') != buf_.end())
	// {
	// 	if (std::find(, buf_.end(), '\n') != buf_.end())
	// }
	// else
	// 	std::cout << "NOT YET" << std::endl;
	// std::vector<unsigned char>::iterator start = buf_.begin();
	// std::vector<unsigned char>::iterator end = buf_.end();
	// std::vector<unsigned char>::iterator pos = std::find(start, end, END_BUF);
	// if (pos == buf_.end())
	// else


	// while (1)
	// {
	// 	Request &request = getRequest();
	// 	if (request.empty())
	// 	{
	// 		printf("yes it's empty\n");

    // 		// request.printRequestMsg();
	// 		break;
	// 	}
	// }

    // this->request_.parsingBuffer();
    // std::cout << this->request_.getBuffer();
    // std::cout << "Recv" << std::endl;
    // ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);

}

void    Connection::response() {
    std::cout <<"Response execute" <<std::endl;
}

Request	&Connection::getRequest(void) {
	return (request_);
}

bool	&Connection::getCtlMode(void) {
	return (Ctl_mode_flag_);
}

void	Connection::setCtlMode(bool mode) {
	Ctl_mode_flag_ = mode;
}
