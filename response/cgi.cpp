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
    isPost_ = ((request_.getMethod() == "POST") && (request_.getBody().size() > 0));
}

Cgi::~Cgi() {
    if (environ_) {
        freeTable(environ_);
        environ_ = NULL;
    }
    if (argvExecve_) {
        freeTable(argvExecve_);
        argvExecve_ = NULL;
    }
    //std::cout << "end of cgi" << std::endl;
}





/* *************************************************** */
/* ********************** make body ****************** */
/* *************************************************** */
// req_code in parametre
std::string            Cgi::makeBodyCgi(int &reqStatusCode) {
    
    // body_ : string pour return
    std::string body_("");

    // verifier format de cgi et cgi path
    if ( ! (isFormatCgi() && isFormatCgiPath())) {
      reqStatusCode = 500;
    }
    else {
        // make environ, parametre pour function execve
        
        reqStatusCode = setVariable();

        /* refaire de setting evirons */
        //printTable(environ_);

        if (reqStatusCode < 400) {
            // setting for pipe, fork... etc
            setPipe();
            childPid_ = fork();

            if (childPid_ == -1) {
                reqStatusCode = 500;
                return body_;
            }
            else if (childPid_ == 0) {
                executeChildProcess();
            }
            else {
                if (request_.getMethod() == "GET") {
                    wait(NULL);
                }
                body_ += executeParentProcess();
            }
            // child process
            // parent process
            // write to child process, if method -> POST
            // read to parent process!
            // mette a body_
            // close pipe;
            reqStatusCode = 201;
        }
    } 

    // status code retun and update for req_code;

    //return body_
    return body_;
}

void                    Cgi::executeChildProcess(void) {
    
    
    // retExecute;
    int     retExecute;
    
    // close fd
    if (isPost_) {
        close(writeToCgi_);
    }
    close(readFromCgi_);

    // dup
    if (isPost_) {
        dup2(stdinCgi_, STDIN_FILENO);
    }
    dup2(stdoutCgi_, STDOUT_FILENO);
    
    // execve
    retExecute = execve(const_cast<char*>(location_.getCgiPath().c_str()), argvExecve_, environ_);
    if (retExecute  == -1)
    {
        closePipe();
        return ;
    }
}

std::string             Cgi::executeParentProcess(void) {
    std::string     body_("");

    
    //cloase fd
    if (isPost_) {
        close(stdinCgi_);
    }
    close(stdoutCgi_);

    // write to Cgi
    if (isPost_) {
        writeToCgi();
        close(writeToCgi_);
    }
    body_ += readFromCgi();
    
    //close fd
    close(readFromCgi_);
    return body_;
}

void                    Cgi::writeToCgi(void) {

    std::string    body_(request_.getBody());
    

    char    *buf_ = const_cast<char*>(request_.getBody().c_str());
    int     size_(request_.getBody().size());
    int     retWrite_;

    //do {
        retWrite_ = write(writeToCgi_, buf_, size_);
    //} while (retWrite_ > 0);
}

std::string             Cgi::readFromCgi(void) {
    


    if (request_.getMethod() == "POST") {
        wait(NULL);
    }

    std::string     body_;
    char            buf_[65536 + 1];
    int             retRead_;

   
   do {
      memset(buf_, 0, 65536);
      retRead_ = read(readFromCgi_, buf_, sizeof(buf_));
      body_ += buf_;

    } while (retRead_ > 0);
    //std::cout << "read From Cgi == [" << retRead_ << "]" << std::endl;
    return body_;
}



/* *************************************************** */
/* ********************** initial ******************** */
/* *************************************************** */
void                    Cgi::initialPipe(void) {
    environ_ = NULL;
    argvExecve_ = NULL;
}

void                    Cgi::setPipe(void) {
    if (pipe(pipeRead_) < 0) {
        std::cerr << "error open pipe" << std::endl;
        return ;
    }

    if (isPost_) {
        if (pipe(pipeWrite_) < 0) {
            std::cerr << "error open pipe" << std::endl;
            close(pipeRead_[0]);
            close(pipeRead_[1]);
            return ;
        }
        writeToCgi_ = pipeWrite_[1];
        stdinCgi_ = pipeWrite_[0];
        // non block
        fcntl(writeToCgi_, F_SETFL, O_NONBLOCK);
        fcntl(stdinCgi_, F_SETFL, O_NONBLOCK);
    }

    readFromCgi_ = pipeRead_[0];
    stdoutCgi_ = pipeRead_[1];

    // non block
    fcntl(readFromCgi_, F_SETFL, O_NONBLOCK);
    fcntl(stdoutCgi_, F_SETFL, O_NONBLOCK);

    //std::cout << "pipe == [" << writeToCgi_ << "]" << std::endl;
    //std::cout << "pipe == [" << stdinCgi_ << "]" << std::endl;
    //std::cout << "pipe == [" << readFromCgi_ << "]" << std::endl;
    //std::cout << "pipe == [" << stdoutCgi_ << "]" << std::endl;
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
    char            *cgiPath_ = const_cast<char*>(cgiPathTmp_.c_str());

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

 void                       Cgi::freeTable(char **table) {
    int i(0);
    while (table[i]) {
        free(table[i]);
        i++;
    }
    free(table);
 }

std::string                 Cgi::getLast(std::string const &str, std::string const &cut) {
    std::string         ret(str);
    
    std::string::size_type n = str.rfind(cut);
    if (cut == "?" && std::string::npos == n) {
        return ("");
    }
    if (std::string::npos != n) {
        ret.clear();
        ret = str.substr(n + 1);
    }
    return str;
}

void                        Cgi::closePipe(void) {
    if (stdinCgi_ != -1) {
        close(stdinCgi_);
    }
    if (stdoutCgi_ != -1) {
        close(stdoutCgi_);
    }
    if (readFromCgi_ != -1) {
        close(readFromCgi_);
    }
    if (writeToCgi_ != -1) {
        close(writeToCgi_);
    }
}


/* *************************************************** */
/* ********************** setter ********************* */
/* *************************************************** */
int                        Cgi::setVariable(void) {
    int     statusCode_(SUCCESS);

    if ((statusCode_ = setEnviron()) != SUCCESS)
        return statusCode_; 
    if ((statusCode_ = makeArgvForExecve()) != SUCCESS)
        return statusCode_;

    //std::cout << "**********start**********" << std::endl;
    //std::cout << "******** environ *********" << std::endl;
    //printTable(environ_);
    //std::cout << "********* argv **********" << std::endl;
    //printTable(argvExecve_);
    //std::cout << "***********end************" << std::endl;

    return statusCode_;
}

 int                       Cgi::makeArgvForExecve(void) {
    int             statusCode_(SUCCESS);
    std::string     cgiPath_(location_.getCgiPath());
    argvExecve_ = (char**)malloc(sizeof(char*) * 3);
    if (!argvExecve_) {
        return 500;
    }
    // get cgi script
    std::string::size_type n = location_.getCgiPath().rfind("/");
    if (n != std::string::npos) {
        cgiPath_ = location_.getCgiPath().substr(n + 1);
    }
    // set argv 
    argvExecve_[2] = NULL;
    argvExecve_[0] = strdup( const_cast<char*>(cgiPath_.c_str()) );
    argvExecve_[1] = strdup( const_cast<char*>(request_.getFilePath().c_str()));

    return statusCode_; 
 }

int                        Cgi::setEnviron(void) {
   int          retCode_(SUCCESS);
   mapEnviron   mapEnviron_(makeMapEnviron());

   // changer mapEnviron_ comme char** environ;
   // si probleme, on doit envoyer status code
   if ((retCode_ = environMapToTable(mapEnviron_)) != SUCCESS) {
        return retCode_;
   }
   return (retCode_);

}

Cgi::mapEnviron                Cgi::makeMapEnviron(void) {
    mapEnviron          mapEnviron_;
    std::string         pathInfo_ = getLast(request_.getFilePath(), "/");
    std::string         uri_ = getLast(request_.getUri(), "?");
    std::string         pathTranslated_(getenv("PWD"));
    pathTranslated_ +=  "/" + request_.getFilePath();
    
    // https://bz.apache.org/bugzilla/show_bug.cgi?id=62663
    mapEnviron_.insert(std::make_pair("AUTH_TYPE", ""));
    mapEnviron_.insert(std::make_pair("REMOTE_IDENT", request_.getMethod()));
    mapEnviron_.insert(std::make_pair("REMOTE_USER", request_.getMethod()));
    mapEnviron_.insert(std::make_pair("REQUEST_METHOD", request_.getMethod()));
    mapEnviron_.insert(std::make_pair("REDIRECT_STATUS", "CGI"));
    mapEnviron_.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
    mapEnviron_.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
    mapEnviron_.insert(std::make_pair("REMOTE_ADDR", "127.0.0.1"));
    mapEnviron_.insert(std::make_pair("SERVER_PORT", toString(server_.getListen())));
    mapEnviron_.insert(std::make_pair("SCRIPT_NAME", location_.getCgiPath()));
    if (!request_.getQueryString().empty() && request_.getMethod() == "GET") {
        mapEnviron_.insert(std::make_pair("QUERY_STRING", request_.getQueryString()));
    }
    mapEnviron_.insert(std::make_pair("SERVER_SOFTWARE", "Webserv"));
    if (!request_.getBody().empty() && request_.getMethod() == "POST") {
        mapEnviron_.insert(std::make_pair("CONTENT_LENGTH", toString(request_.getBody().size())));
    }
    mapEnviron_.insert(std::make_pair("CONTENT_TYPE",  "application/x-www-form-urlencoded"));
    mapEnviron_.insert(std::make_pair("SCRIPT_FILENAME", pathTranslated_));
    mapEnviron_.insert(std::make_pair("PATH_TRANSLATED", pathTranslated_));
    mapEnviron_.insert(std::make_pair("PATH_INFO", pathInfo_));

    // check! justement pour get??
    if ( !uri_.empty() && request_.getMethod() == "GET") {
        mapEnviron_.insert(std::make_pair("REQUEST_URI", uri_));
    }

    printmap(mapEnviron_);
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
        tmpEnv_ = NULL;
        tmp_.clear();
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

void                        Cgi::printTable(char **table) const {
    int     i(0);
   
    while (table[i]) {
        std::cout << table[i++] << std::endl;
    }
}


} // namespace ft