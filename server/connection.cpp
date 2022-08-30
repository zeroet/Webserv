#include "connection.hpp"

Connection::Connection(int fd, std::vector<ServerBlock> block, Epoll *ep) : clntFd_(fd), block_(block), ep_(ep), serverConfig_(), locationConfig_(), request_(), response_() {
	memset(buffer_char, 0, BUFFER_SIZE);
	phase_msg_ = START_LINE_INCOMPLETE;
	req_status_code_ = NOT_DEFINE;
	client_max_body_size = 0;
	buffer_content_length = -1;
	is_chunk = false;
	chunked_msg_checker = STR_SIZE;
	chunked_msg_size = 0;
	body_buf = "";
	autoindex_flag = false;
	status_ = "Keep-Alive";
}

Connection::Connection(const Connection &rhs)
{
	*this = rhs;
}

Connection Connection::operator=(const Connection &rhs)
{
	clntFd_ = rhs.clntFd_;
	block_ = rhs.block_;
	status_ = rhs.status_;
	ep_  = rhs.ep_;
	serverConfig_ = rhs.serverConfig_;
	locationConfig_ = rhs.locationConfig_;
	
	request_ = rhs.request_;
	response_ = rhs.response_;
	phase_msg_ = rhs.phase_msg_;
	req_status_code_ = rhs.req_status_code_;
	buffer_ = rhs.buffer_;
	client_max_body_size = rhs.client_max_body_size;
	buffer_content_length = rhs.buffer_content_length;
	is_chunk = rhs.is_chunk;
	chunked_msg_checker = rhs.chunked_msg_checker;
	chunked_msg_size = rhs.chunked_msg_size;
	body_buf = rhs.body_buf;
	autoindex_flag = rhs.autoindex_flag;

	return *this;
}

Connection::~Connection() {}

void	Connection::clear(void) {
	memset(&buffer_char, 0, BUFFER_SIZE);
	buffer_.clear();
	request_.clear();
	body_buf.clear();
	phase_msg_ = START_LINE_INCOMPLETE;
	req_status_code_ = NOT_DEFINE;
	client_max_body_size = 0;
	buffer_content_length = -1;
	is_chunk = false;
	chunked_msg_checker = STR_SIZE;
	chunked_msg_size = 0;
	autoindex_flag = false;
	locationConfig_ = LocationBlock();
	serverConfig_ = ServerBlock();
}

void    Connection::processRequest(void) {
	RequestHandler requesthandler;
    int n = 0;
	//const char ctrl_c[CTRL_C_LIST] = {0xff, 0xf4, 0xfd, 0x06};

	while((n = recv(this->clntFd_, &buffer_char, sizeof(buffer_char) - 1, 0)) > 0) //except \r
	{
		if (n < 0 || strchr(buffer_char, 0xff))
		{
			close(clntFd_);
			ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
			return ;
		}
 		if (!strcmp(buffer_char, "\r\n") 
				&& n == 2 
				&& phase_msg_ == START_LINE_INCOMPLETE && buffer_.size() == 0) 
		{
			memset(&buffer_char, 0, n);
      		return ;
    	}
		
		buffer_.insert(buffer_.end(), buffer_char, buffer_char + n);

		if (phase_msg_ == START_LINE_INCOMPLETE
			|| phase_msg_ == START_LINE_COMPLETE
			|| phase_msg_ == HEADER_INCOMPLETE
			|| phase_msg_ == HEADER_COMPLETE)
			requesthandler.checkRequestMessage(this);
		if (phase_msg_ == BODY_CHUNKED)
		{
			if (!requesthandler.checkChunkedMessage(this))
			{
				this->status_ = "Close";
				ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
				return ;
			}
		}
		else if (phase_msg_ == BODY_INCOMPLETE)
			requesthandler.checkRequestBody(this);
		if (phase_msg_ == BODY_COMPLETE || phase_msg_ == START_LINE_ERROR)
		{
			if (phase_msg_ == START_LINE_ERROR) {
				ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
			}
			if (getRequest().getMethod() == "GET" || getRequest().getMethod() == "DELETE")
			{
				if (buffer_.empty())
					ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
			}
			else
			{
				request_.setBody(getBodyBuf());
				ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
			}

		}
		memset(&buffer_char, 0, n);
	}
}

void    Connection::processResponse()
{
	std::string			returnBuffer_("");
	std::string			header_("");
	std::string			body_("");
	std::string			currentMethod_(request_.getMethod());
	std::string			Ext_(response_.getExt(request_.getFilePath()));
	MimeType			mime_;
	bool				isGetHTML(currentMethod_ == "GET" && Ext_ != "php");
	bool				isHTMLMimeType_(mime_.getMIMEType(Ext_) == "text/html");
	bool				falgHeaderRedirection_(false);

	// intializer les valeurs de Request class
	response_.setRequest(request_);
	response_.setRequestValue();
	response_.setLocation(getLocationConfig());

	if (!locationConfig_.getReturn().empty()) {
		if (req_status_code_ == 0) {
			req_status_code_ = 301;
			falgHeaderRedirection_ = true;
		}
	}
	else if (req_status_code_== NOT_DEFINE) {
		if (currentMethod_ == "GET" || currentMethod_ == "POST") {
				if (isGetHTML) {
					if (autoindex_flag) {
						struct stat			fileinfo;
						stat(request_.getFilePath().c_str(), &fileinfo);
						if (S_ISDIR(fileinfo.st_mode))
						{
							body_ = response_.bodyWithAutoindexOn(request_.getPath(), request_.getFilePath());
							req_status_code_ = 200;
						}
						else if (S_ISREG(fileinfo.st_mode))
						{
							body_ = response_.fileTextIntoBody(request_.getFilePath());
							req_status_code_ = 200;
						}
					}
					else
					{
						body_ = response_.makeBodyHtml(request_.getFilePath(), isHTMLMimeType_);
						req_status_code_ = 200;
					}
				}
				else {
					Cgi		cgi_(getServerConfig(), getLocationConfig()
									, request_, buffer_content_length);
					
					body_ = cgi_.makeBodyCgi(req_status_code_);
				}
		}
		else if (currentMethod_ == "DELETE") {
			req_status_code_ = response_.execteDelete();
		}
	}

	// error page
	if (req_status_code_ >= 400) {
		body_ += response_.makeErrorPage(req_status_code_);
		status_ = "Close";
	}
	
	// make header_
	// si method est "POST" et error page, ajouter header
	// sinon, cgi rends header
	if (isGetHTML || req_status_code_ >= 400) {
		header_ += response_.makeHeader(body_.size(), req_status_code_);
	}
	else {
		header_ += response_.makeHeaderCgi(body_, req_status_code_);
	}


	// header for redirection
	if (!locationConfig_.getReturn().empty() && falgHeaderRedirection_) {
		header_ += "Location: ";
		header_ += body_buf;
		header_ += "\r\n";
	}

	// status code
	if (!request_.getHeaderValue("Connection").empty()) {
		if (request_.getHeaderValue("Connection") == "close") {
			status_ = "Close";
			header_ += "Connection: close\r\n";
		}
	}
	else if (req_status_code_ < 400) {
		status_ = "Keep-Alive";
		header_ += "Connection: keep-alive\r\n";
	}

	// make return buffer
	returnBuffer_ = header_ + body_ + "\r\n";
	
	// send return buffer
	int	n;	
	int	size_(returnBuffer_.size());
	do {
		n = send(clntFd_, const_cast<char*>(returnBuffer_.c_str()), returnBuffer_.size(), 0);
		size_ -= n;
	} while (n > 0 && size_ > 0);
	if (n < 0) {
		std::cerr << "Error send" << std::endl;
		status_ = "Error";
	}
	// clean up buffer
	header_.clear();
	body_.clear();
	returnBuffer_.clear();
	clear();
}

//getter
std::vector<ServerBlock>		&Connection::getBlock(void) {
	return (block_);
}

Request	&Connection::getRequest(void) {
	return (request_);
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

ServerBlock	Connection::getServerConfig(void) {
	return (serverConfig_);
}

LocationBlock	Connection::getLocationConfig(void) {
	return (locationConfig_);
}

std::string		&Connection::getBodyBuf(void) {
	return (body_buf);
}

std::string		Connection::getStatus(void)
{
	return (this->status_);
}


//setter
void	Connection::setReqStatusCode(int status_code) {
	req_status_code_ = status_code;
}

void	Connection::setPhaseMsg(int new_msg) {
	phase_msg_ = new_msg;
}

void	Connection::setServerBlockConfig(std::string server_name) {
	serverConfig_ = getServerConfigByServerName(server_name);
}

void	Connection::setLocationConfig(LocationBlock locationblock) {
	locationConfig_ = locationblock;
}

void	Connection::setBodyBuf(std::string bodybuf) {
	body_buf = bodybuf;
}


bool		Connection::checkLocationConfigExist(std::string path) {
	std::pair<bool, LocationBlock> location_pair;

	location_pair = serverConfig_.getLocationBlock(path);
	if (location_pair.first == true)
	{
		setLocationConfig(location_pair.second);
		return (true);
	}
	else
		return (false);
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
	printf("%s\n", getBodyBuf().c_str());
	printf("=====================\n");
}

ServerBlock	Connection::getServerConfigByServerName(std::string server_name)
{
	int index = this->block_.size();
	bool ret;

	for (int i = 0; i < index; i++)
	{
		ret = block_[i].checkServerName(server_name);
		if (ret == true)
		{
			return (block_[i]);
		}
	}
	return (block_[0]);
}