#include "connection.hpp"

OperateRequest operateRequest = OperateRequest();

Connection::Connection(int fd, ServerBlock block, Epoll *ep) : clntFd_(fd), block_(block), ep_(ep), request_(new Request) {
	// Ctl_mode_flag_ = false;
	phase_msg_ = START_LINE_INCOMPLETE;
	req_status_code_ = NOT_DEFINE;
	content_length = 0;
	client_max_body_size = 0;
	is_chunk = false;
}

Connection::~Connection() { }

void    Connection::processRequest()
{

	memset(&buffer_char, 0, BUFFER_SIZE);
    int n = recv(this->clntFd_, &buffer_char, sizeof(buffer_char) - 1, 0); //except \r

	/* protection to disconnect
	if (n < 0 || strchr(buffer_char, 0xff))
	{
		//close connection
		printf("recv Error\n");
		return ;
		//need function to close connection
		//return (Error); or return (-1);
	}*/
	// std::cout << "n: " << n << std::endl;
	// std::cout << "buffer_char: " << buffer_char << std::endl;
	// std::cout << "buffer_char + n: " << buffer_char + n << std::endl;
    // buf_.insert(buf_.end(), buffer_char, buffer_char + n);
	buffer_.insert(buffer_.end(), buffer_char, buffer_char + n);
	std::cout << "buffer_: " << buffer_ << std::endl;

	if (phase_msg_ == START_LINE_INCOMPLETE
		|| phase_msg_ == START_LINE_COMPLETE
		|| phase_msg_ == HEADER_INCOMPLETE
		|| phase_msg_ == HEADER_COMPLETE)
		operateRequest.checkRequestMessage(this);

	if (phase_msg_ == BODY_COMPLETE)
		std::cout << "************ Message body process **********" << std::endl;


	// std::vector<std::string> ret = getBlock().locationList[0].getReturn();

	// for (size_t i = 0; i < ret.size(); i++)
	// 	std::cout << "///////////////////////////////////////////vector value: " << ret[i] << std::endl;

	////////////
	// if (phase_msg_ == BODY_COMPLETE)
	// {
	// 	std::cout << "CGI / EXECUTE / RESPONSE NEED TO BE DEAL" << std::endl;
	// 	// ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
	// }
	////////////

	//to change Ctl Mode when message is done with CRLFCRLF
	size_t pos = 0;
	if ((pos = buffer_.find(CRLFCRLF)) != std::string::npos) {
		if (buffer_.empty())
			ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
	}
	// if (n == 0 && buffer_.empty() && phase_msg_ == BODY_COMPLETE)
}

//getter
ServerBlock		&Connection::getBlock(void) {
	return (block_);
}

Request	&Connection::getRequest(void) {
	return (*request_);
}

Response	&Connection::getResponse(void) {
	return (response_);
}

int			&Connection::getReqStatusCode(void) {
	return (req_status_code_);
}

int		&Connection::getPhaseMsg(void) {
	return (phase_msg_);
}

std::string	&Connection::getBuffer(void) {
	return (buffer_);
}

//setter
void	Connection::setReqStatusCode(int status_code) {
	req_status_code_ = status_code;
}

void	Connection::setPhaseMsg(int new_msg) {
	phase_msg_ = new_msg;
}

void    Connection::response() {
    std::cout <<"Response execute" <<std::endl;
}

//tmp
void	Connection::printRequestMsg(void) {
	printf("====Request Parser====\n");
	printf("Request_status_code: %d\n", getReqStatusCode());
	printf("Phase_line: %d\n", getPhaseMsg());
	printf("method_: %s\n", getRequest().getMethod().c_str());
	printf("uri_: %s\n", getRequest().getUri().c_str());
	printf("path_: %s\n", getRequest().getPath().c_str());
	printf("file_path_: %s\n", getRequest().getFilePath().c_str());
	printf("host_: %s\n", getRequest().getHost().c_str());
	printf("port_: %s\n", getRequest().getPort().c_str());

	printf("version_: %s\n", getRequest().getVersion().c_str());
	getRequest().printHeaders();
	std::cout << std::endl;
	printf("=====================\n");
	printf("body:\n");
	printf("%s", getRequest().getBody().c_str());
	printf("=====================\n");
	std::cout << "content_length: " << content_length << std::endl;
	std::cout << "client_max_body_size: " << client_max_body_size << std::endl;
	printf("=====================\n");
}
