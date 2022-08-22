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
    std::string body_("");

    if ( ! (isFormatCgi() && isFormatCgiPath())) {
        reqStatusCode = 500;
    }
    else {

        reqStatusCode = 201;
    } 
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




/* *************************************************** */
/* ********************** initial ******************** */
/* *************************************************** */
bool                    Cgi::isFormatCgi(void) const {
    if ( ! (location_.getCgi().empty())) {
        std::vector<std::string>            cgiList_(location_.getCgi());
        std::vector<std::string>::iterator  itBegin_(cgiList_.begin());
        std::vector<std::string>::iterator  itEnd_(cgiList_.end());
        std::string                         ext_("." + getExt(request_.getFilePath()));
    
        for (;itBegin_ != itEnd_; itBegin_++) {
            if (ext_ == *itBegin_) {
                return true;
            }
        }
    }
    return false;
}

bool                    Cgi::isFormatCgiPath(void) const {
    std::string     cgiPathTmp_(location_.getCgiPath());
    char *cgiPath_ = const_cast<char*>(cgiPathTmp_.c_str());


    if (access(cgiPath_, X_OK) == -1)
        return false;
    return true;
}


/* *************************************************** */
/* ********************** initial ******************** */
/* *************************************************** */
std::string	                Cgi::getExt(std::string const &filename) const
{
    if (filename.empty())
        return NULL;

    std::string	ext;
	std::string::size_type	idx;
	idx = filename.rfind(".");
	if (idx != std::string::npos) {
		ext = filename.substr(idx + 1);
	}
    return ext;
}

} // namespace ft