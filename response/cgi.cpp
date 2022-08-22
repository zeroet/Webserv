#include "cgi.hpp"

namespace ft
{

/* *************************************************** */
/* ************* constructor / destructor ************ */
/* *************************************************** */
Cgi::Cgi() {
    initialPipe();
}


Cgi::Cgi(LocationBlock const &location, Request const &request) {
    initialPipe();
    location_ = location;
    request_ = request;
}

Cgi::~Cgi() {}



/* *************************************************** */
/* ********************** make body ****************** */
/* *************************************************** */
// req_code in parametre
std::string            Cgi::makeBodyCgi(int &reqStatusCode) {
    std::string body_;

    body_ += "here is cgi";
    // check format with cgi
    // check cgi-php with cgi_path
    // return with body_ vide;
    // req_code update;
    // make environ;
    // make variable for execve

    // child process
    // parent process

    // status code retun and update for req_code;

    //return body_
    
    reqStatusCode = 201;
    return body_;
}

/* *************************************************** */
/* ********************** initial ******************** */
/* *************************************************** */
void                    Cgi::initialPipe(void) {
    pipeWrite_[0] = -1;
    pipeWrite_[1] = -1;
    pipeRead_[0] = -1;
    pipeRead_[1] = -1;
}

void                    Cgi::initialEnviron(void) {
    
}

} // namespace ft