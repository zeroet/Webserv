#ifndef CGI_HPP
# define CGI_HPP

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <iostream>
# include "../config/src/ServerBlock.hpp"
# include "../config/src/LocationBlock.hpp"
# include "../config/src/ServerBlock.hpp"
# include "../request/request.hpp"
# include <vector>
# include <map>
# include <sstream>

namespace ft
{

# define    PIPE_IN     1
# define    PIPE_OUT    0
# define    SUCCESS     1

class ServerBlock;
class LocationBlock;
class Request;

class Cgi
{
public:
    typedef std::map<std::string, std::string>  mapEnviron;

private:
    /* ******************************** */
    /* *** pipe for write and read **** */
    /* ******************************** */
    int                 pipeWrite_[2];
    int                 pipeRead_[2];
    int                 writeToCgi_;
    int                 readFromCgi_;
    int                 stdinCgi_;
    int                 stdoutCgi_;

    /* ******************************** */
    /* * variables for child process ** */
    /* ******************************** */
    int                 childPid_;
    char                **environ_;
    char                **argvExecve_;

    /* ******************************** */
    /* *** set Location / request ***** */
    /* ******************************** */
    LocationBlock       location_;  
    Request             request_;
    ServerBlock         server_;


public:
    /* ******************************** */
    /* *** constructor / destructor *** */
    /* ******************************** */
    Cgi();
    Cgi(ServerBlock const &server, LocationBlock const &location, Request const &request);
    ~Cgi();

    /* ******************************** */
    /* *********** make body ********** */
    /* ******************************** */
    std::string         makeBodyCgi(int &reqStatusCode);
    void                executeChildProcess();
    std::string         executeParentProcess();
    
private:
    /* ******************************** */
    /* ************ initial *********** */
    /* ******************************** */
    void                initialPipe(void);
    void                setPipe(void);
    void                writeToCgi(void);
    std::string         readFromCgi(void);

    /* ******************************** */
    /* ************ checker *********** */
    /* ******************************** */
    bool                isFormatCgi(void) const;
    bool                isFormatCgiPath(void) const;

    /* ******************************** */
    /* ************ utils ************* */
    /* ******************************** */
    std::string         getExt(std::string const &filename) const;
    std::string         toString(const int& v) const;
    void                freeTable(char **table);

    /* ******************************** */
    /* ************ setter ************ */
    /* ******************************** */
    int                 setVariable(void);
    int                 setEnviron(void);
    mapEnviron          makeMapEnviron(void);
    int                 environMapToTable(mapEnviron &mapEnviron_);
    int                 makeArgvForExecve(void);

    /* ******************************** */
    /* ************ tester ************ */
    /* ******************************** */
    void                printmap(ft::mapHeader mapHeader_) const;
    void                printTable(char **table) const;
};

} // namespace ft

#endif