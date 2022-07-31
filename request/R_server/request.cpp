#include "request.hpp"

Request::Request(void) : method_(""), path_(""), version_(""), requestHeaders_(), body_() {} 

Request::Request(Request const &x) {
	*this = x;
}

const	Request	&Request::operator=(const Request &x) {
	if (this != &x) {
    method_ = x.method_;
    path_ = x.path_;
	version_ = x.version_;
    requestHeaders_ = x.requestHeaders_;
    body_ = x.body_;
  }
  return *this;
}

Request::~Request(void) {}

//getter
const	std::string	&Request::getMethod(void) const {
	return (method_);
}

const	std::string &Request::getPath(void)	const {
	return (path_);
}

const	std::string	&Request::getVersion(void)	const {
	return (version_);
}

const	mapHeader	&Request::getRequestHeaders(void)	const {
	return (requestHeaders_);
}

const	std::string	Request::getHeaderValue(std::string key) const {
	std::transform(key.begin(), key.end(), key.begin(), toupper);
	for (mapHeader::const_iterator it = requestHeaders_.begin(); it != requestHeaders_.end(); it++)
	{
		if (it->first == key)
			return (it->second);
	}
	return (NULL);
}

const	vecByte		&Request::getBody(void)	const {
	return (body_);
}

//setter

// void    Request::addBuffer(void)
// {
//     // memset(&buf, 0, sizeof(buf));
//     int n = recv(this->fd_, &buffer, sizeof(buffer), 0); 
// 	// std::cout << "i from recv :" << n << std::endl;
// 	if (n == -1)
// 	{
// 		printf("Error recv\n");
// 		//need function to quit
// 		//return (Error); or return (-1);
// 	}
// 	std::cout << "n: " << n << std::endl;
//     buf_.insert(buf_.end(), buffer, buffer + n);
// 	std::cout << "buf_: " << std::endl;
// 	for (unsigned int i = 0; i < buf_.size(); i++)
// 	{
// 		std::cout << buf_[i];
// 	}
// 	// while(1) {

// 	// }

// }

// void    Request::parsingBuffer(void)
// {
//     std::string tmp;
    // std::string::size_type posEndBuff(-1);
    // std::string::size_type posCRLF(-1);

    // std::cout << posEndBuff << " est pos End Buff" <<  std::endl;
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
// }

// std::string const &Request::getBuffer(void) const
// std::vector<unsigned char> &Request::getBuffer(void) {
//     return (this->buf_);
// }