#include "connection.hpp"

//RequestHandle		requesthandler = RequestHandler();

Connection::Connection(int fd, std::vector<ServerBlock> block, Epoll *ep) : clntFd_(fd), block_(block), ep_(ep), request_() {
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
}

Connection::~Connection() { }

void	Connection::clear(void) {
	request_.clear();
	// response_.clear();
	phase_msg_ = START_LINE_INCOMPLETE;
	req_status_code_ = NOT_DEFINE;
	client_max_body_size = 0;
	buffer_content_length = -1;
	is_chunk = false;
	chunked_msg_checker = STR_SIZE;
	chunked_msg_size = 0;
	body_buf.clear();
	autoindex_flag = false;
}

void    Connection::processRequest(void) {
	RequestHandler requesthandler;
    int n = 0;
	
	while((n = recv(this->clntFd_, &buffer_char, sizeof(buffer_char) - 1, 0)) > 0) //except \r
	{
		// /* protection to disconnect
		if (n < 0 || strchr(buffer_char, 0xff))
		{
			//close connection
			this->status_ = "Close";
			return ;
			//need function to close connection
			//return (Error); or return (-1);
		}
		// std::cout << "n: " << n << std::endl;
		// std::cout << "buffer_char: " << buffer_char << std::endl;
		// std::cout << "buffer_char + n: " << buffer_char + n << std::endl;
		// buf_.insert(buf_.end(), buffer_char, buffer_char + n);
		buffer_.insert(buffer_.end(), buffer_char, buffer_char + n);
		// buffer_.append(buffer_char);
		// std::cout << "buffer_: " << buffer_ << std::endl;

		if (phase_msg_ == START_LINE_INCOMPLETE
			|| phase_msg_ == START_LINE_COMPLETE
			|| phase_msg_ == HEADER_INCOMPLETE
			|| phase_msg_ == HEADER_COMPLETE)
			// || phase_msg_ == BODY_CHUNKED)
			requesthandler.checkRequestMessage(this);
		if (phase_msg_ == BODY_CHUNKED)
		{
			if (!requesthandler.checkChunkedMessage(this))
			{
				this->status_ = "Close";
				return ;
			}
		}
		else if (phase_msg_ == BODY_INCOMPLETE)
			requesthandler.checkRequestBody(this);
		if (phase_msg_ == BODY_COMPLETE)
		{
			std::cout << "************ Message body process **********" << std::endl;
//			size_t pos = 0;
			if (getRequest().getMethod() == "GET" || getRequest().getMethod() == "DELETE")
			{
				if (buffer_.empty())
				{
					ep_->epoll_Ctl_Mode(clntFd_, EPOLLOUT);
					std::cout << "HERE" << std::endl;
				}
			}
			else
			{
				request_.setBody(getBodyBuf());
//				if ((pos = buffer_.find(CRLFCRLF)) != std::string::npos)
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
						body_ = response_.bodyWithAutoindexOn(request_.getPath(), request_.getFilePath());
						req_status_code_ = 200;
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
	if (!locationConfig_.getReturn().empty() && falgHeaderRedirection_) {
		header_ += "Location: ";
		header_ += body_buf;
		header_ += "\r\n";
	}
	// make return buffer
	returnBuffer_ = header_ + body_ ;

	// send return buffer
	send(clntFd_, const_cast<char*>(returnBuffer_.c_str()), returnBuffer_.size(), 0);

	// clean up buffer
	header_.clear();
	body_.clear();
	returnBuffer_.clear();

	// close connection  client fd
	//
	ep_->end_connection(clntFd_);
	//status_ = "Close";
	// epollout, close fd
	//ep_->epoll_Ctl_Mode(clntFd_, EPOLLIN);
	
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
	// il faut faire protection pour content-length

	//std::istringstream		contentLength(request_.getHeaderValue("Content-Length"));
	//int						contentLength_;
	//contentLength >> contentLength_;
	//body_buf.resize(contentLength_);
	
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
	std::cout << "content_length_buffer: " << buffer_content_length << std::endl;
	// std::cout << "content_length: " << getRequest().getHeaderValue("Content-Length") << std::endl;
	std::cout << "client_max_body_size: " << client_max_body_size << std::endl;
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

