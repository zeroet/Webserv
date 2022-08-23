#include "cgi.hpp"

namespace ft
{


/* *************************************************** */
/* ************* constructor / destructor ************ */
/* *************************************************** */
Cgi::Cgi() {
    initialPipe();
}


Cgi::Cgi(ServerBlock const &server, LocationBlock const &location, Request const &request) {
    initialPipe();
    location_ = location;
    request_ = request;
    server_ = server;
}

Cgi::~Cgi() {
    freeEnviron();
}





/* *************************************************** */
/* ********************** make body ****************** */
/* *************************************************** */
// req_code in parametre
std::string            Cgi::makeBodyCgi(int &reqStatusCode) {
    // body_ : string pour return
    std::string body_("\r\nbody part");

    // verifier format de cgi et cgi path
    if ( ! (isFormatCgi() && isFormatCgiPath())) {
        reqStatusCode = 500;
    }
    else {
        // make environ, parametre pour function execve
        
        reqStatusCode = setEnviron();
        if (reqStatusCode < 400) {

            // child process
            // parent process
                
            reqStatusCode = 201;
        }
    } 

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
    environ_ = NULL;
}

void                    Cgi::initialEnviron(void) {
    
}




/* *************************************************** */
/* ********************** checker********************* */
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
/* ********************** utils ********************** */
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

std::string                 Cgi::toString(const int& v) const {
	std::ostringstream ss;
	ss << v;
	return (ss.str());
}

 void                       Cgi::freeEnviron(void) {
    int i(0);
    while (environ_[i]) {
        free(environ_[i]);
    }
    free(environ_);
 }

/* *************************************************** */
/* ********************** setter ********************* */
/* *************************************************** */
int                        Cgi::setVariable(void) {
    int     statusCode_;

    if ((statusCode_ = setEnviron()) != SUCCESS)
        return statusCode_; 

    return (SUCCESS);
}


int                        Cgi::setEnviron(void) {
   int          retCode(SUCCESS);
   mapEnviron   mapEnviron_(makeMapEnviron());

   // changer mapEnviron_ comme char** environ;
   // si probleme, on doit envoyer status code
   if ((retCode = environMapToTable(mapEnviron_)) != SUCCESS) {
        return retCode;
   }
   // make command
   return (retCode);

}

Cgi::mapEnviron                Cgi::makeMapEnviron(void) {
    mapEnviron  mapEnviron_;

    // https://bz.apache.org/bugzilla/show_bug.cgi?id=62663
    mapEnviron_.insert(std::make_pair("REQUEST_METHOD", request_.getMethod()));
    mapEnviron_.insert(std::make_pair("REDIRECT_STATUS", "CGI"));
    mapEnviron_.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
    mapEnviron_.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
    mapEnviron_.insert(std::make_pair("REMOTE_ADDR", "127.0.0.1"));
    mapEnviron_.insert(std::make_pair("SERVER_PORT", toString(server_.getListen())));
    mapEnviron_.insert(std::make_pair("SCRIPT_NAME", location_.getCgiPath()));
    if (!request_.getQueryString().empty() && request_.getMethod() == "GET")
        mapEnviron_.insert(std::make_pair("QUERY_STRING", request_.getQueryString()));
    mapEnviron_.insert(std::make_pair("SERVER_SOFTWARE", "Webserv"));
    if (!request_.getBody().empty() && request_.getMethod() == "POST")
        mapEnviron_.insert(std::make_pair("CONTENT_LENGTH", toString(request_.getBody().size())));
    mapEnviron_.insert(std::make_pair("CONTENT_TYPE",  "application/x-www-form-urlencoded"));
    mapEnviron_.insert(std::make_pair("SCRIPT_FILENAME", request_.getFilePath()));
    mapEnviron_.insert(std::make_pair("PATH_TRANSLATED", request_.getFilePath()));
    mapEnviron_.insert(std::make_pair("PATH_INFO", request_.getPath()));
    mapEnviron_.insert(std::make_pair("REQUEST_URI", request_.getUri()));

    return (mapEnviron_);

}


int                             Cgi::environMapToTable(mapEnviron &mapEnviron_) {
    int             it_(0);
    char            *tmpEnv_(NULL);
    std::string     tmp_;

    environ_ = (char**)malloc(sizeof(char*) * (mapEnviron_.size() + 1));
    if (!environ_) {
        return 500;
    }
    environ_[mapEnviron_.size()] = NULL;
    for (mapEnviron::iterator it=mapEnviron_.begin(); it!=mapEnviron_.end(); it++) {
        tmp_ = it->first + "=" + it->second;
        tmpEnv_ = const_cast<char*>(tmp_.c_str());
        environ_[it_] = strdup(tmpEnv_);
        it_++;
    }
    return (SUCCESS);
}



/* *************************************************** */
/* ********************** tester ********************* */
/* *************************************************** */
void				        Cgi::printmap(ft::mapHeader	mapHeader_) const{
	typedef std::map<std::string, std::string>::iterator	it_;

	it_		iter_begin	= mapHeader_.begin();
	it_		iter_end 	= mapHeader_.end();

	for(;iter_begin != iter_end;iter_begin++)
	{
		std::cout << "key == [" << iter_begin->first << "], value == [" \
		<< iter_begin->second << "]" << std::endl;
	}
}

} // namespace ft