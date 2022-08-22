#include "cgi.hpp"

namespace ft
{

/* *************************************************** */
/* ************* constructor / destructor ************ */
/* *************************************************** */
Cgi::Cgi() {
    initialPipe();
}


Cgi::Cgi(LocationBlock const &location) {
    initialPipe();
    location_ = location;
}

Cgi::~Cgi() {}



/* *************************************************** */
/* ********************** make body ****************** */
/* *************************************************** */
std::string            Cgi::makeBodyCgi(void) {
    std::string body_;

    body_ += "here is cgi";

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