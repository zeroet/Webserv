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
# include "../request/request.hpp"
# include <vector>

namespace ft
{

# define    PIPE_IN     1
# define    PIPE_OUT    0

class LocationBlock;
class Request;

class Cgi
{
private:
    /* ******************************** */
    /* *** pipe for write and read **** */
    /* ******************************** */
    int                 pipeWrite_[2];
    int                 pipeRead_[2];

    /* ******************************** */
    /* * variables for child process ** */
    /* ******************************** */
    //int                 childPid_;
    //char                **environ_;

    /* ******************************** */
    /* ********** set Location ******** `*/
    /* ******************************** `*/
    LocationBlock       location_;  
    Request             request_;

public:

    /* ******************************** */
    /* *** constructor / destructor *** */
    /* ******************************** */
    Cgi();
    Cgi(LocationBlock const &location, Request const &request);
    ~Cgi();

    /* ******************************** */
    /* *********** make body ********** */
    /* ******************************** */
    std::string         makeBodyCgi(int &reqStatusCode);
    
private:
    /* ******************************** */
    /* ************ initial *********** */
    /* ******************************** */
    void                initialPipe(void);
    void                initialEnviron(void);

    /* ******************************** */
    /* ************ checker *********** */
    /* ******************************** */
    bool                isFormatCgi(void) const;
    bool                isFormatCgiPath(void) const;


    /* ******************************** */
    /* ************ checker *********** */
    /* ******************************** */
    std::string         getExt(std::string const &filename) const;
    
};

} // namespace ft

#endif