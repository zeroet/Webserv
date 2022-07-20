#include "Request.hpp"

RecvRequest::RecvRequest(void) {}

RecvRequest::RecvRequest(int client_fd) : buf_(0) {
    this->client_fd_ = client_fd;
}

RecvRequest::~RecvRequest(void) {}

int const &RecvRequest::get_client_fd(void) const {
    return (this->client_fd_);
}

char	&RecvRequest::get_buf(void) {
    return (this->buf_);
}

void        RecvRequest::set_buf(char buf) {
    this->buf_ = buf;
}

