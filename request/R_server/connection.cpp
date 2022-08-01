#include "connection.hpp"

Connection::Connection(int fd, Block block, Epoll *ep) : clntFd_(fd), block_(block), ep_(ep) {
    std::cout << ep_->getepollfd() << std::endl;
	Ctl_mode_flag_ = false;
	phase_msg_ = START_LINE_INCOMPLETE;
}

Connection::~Connection() { }

void    Connection::processRequest()
{

	memset(&buffer_char, 0, sizeof(char));
    int n = recv(this->clntFd_, &buffer_char, sizeof(buffer_char) - 1, 0); //except \r
	if (n <= 0 || strchr(buffer_char, 0xff))
	{
		//close connection
		printf("recv Error\n");
		return ;
		//need function to close connection
		//return (Error); or return (-1);
	}
	// std::cout << "n: " << n << std::endl;
	// std::cout << "buffer_char: " << buffer_char << std::endl;
	// std::cout << "buffer_char + n: " << buffer_char + n << std::endl;
    // buf_.insert(buf_.end(), buffer_char, buffer_char + n);
	buffer_.insert(buffer_.end(), buffer_char, buffer_char + n);
	std::cout << "buffer_: " << buffer_ << std::endl;

	size_t pos = 0;
	if (getPhaseMsg() == START_LINE_INCOMPLETE)
	{
		if ((pos = buffer_.find(CRLF) )!= std::string::npos)
		{
			std::cout << "pos: " << pos << std::endl;
			// std::cout << buffer_.substr(pos, ) << std::endl;
			setPhaseMsg(START_LINE_COMPLETE);
		}
	}
	// if (pos2 != 0)
	if (getPhaseMsg() == START_LINE_COMPLETE)
	{
		//parse start line
		std::cout << "Parse Start Line" << std::endl;
	// 	// if ((pos = buffer_.find(CRLFCRLF)) != std::string::npos)
	// 	// std::cout << pos2 << std::endl;
	// 	// std::cout << buffer_.substr(pos2, buffer_.length()) << std::endl;
		if ((pos = buffer_.find(CRLFCRLF)) == std::string::npos)
			setPhaseMsg(HEADER_INCOMPLETE);
	}
	if (getPhaseMsg() == HEADER_INCOMPLETE)
	{
	// 	size_t pos = 0;
		if ((pos = buffer_.find(CRLFCRLF)) != std::string::npos)
		{
			std::cout << "pos: " << pos << std::endl;
	// 	std::cout << buffer_.substr(pos2, buffer_.length()) << std::endl;
			setPhaseMsg(HEADER_COMPLETE);
		}
	}
	if (getPhaseMsg() == HEADER_COMPLETE)
	{
		//parse header
		std::cout << "Parse Header" << std::endl;
	}

	//to change Ctl Mode when message is done with CRLFCRLF
	// size_t pos;
	if ((pos = buffer_.find(CRLFCRLF)) != std::string::npos)
		ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);


}

//getter
Request	&Connection::getRequest(void) {
	return (request_);
}

// bool	&Connection::getCtlMode(void) {
// 	return (Ctl_mode_flag_);
// }

int		&Connection::getPhaseMsg(void) {
	return (phase_msg_);
}

std::string	&Connection::getBuffer(void) {
	return (buffer_);
}

//setter
// void	Connection::setCtlMode(bool mode) {
// 	Ctl_mode_flag_ = mode;
// }

void	Connection::setPhaseMsg(int new_msg) {
	phase_msg_ = new_msg;
}

void    Connection::response() {
    std::cout <<"Response execute" <<std::endl;
}
