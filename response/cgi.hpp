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

namespace ft
{

# define    PIPE_IN     1
# define    PIPE_OUT    0

class LocationBlock;

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
    int                 childPid_;
    char                **environ_;

    /* ******************************** */
    /* ********** set Location ******** `*/
    /* ******************************** `*/
    LocationBlock       location_;  

public:

    /* ******************************** */
    /* *** constructor / destructor *** */
    /* ******************************** */
    Cgi();
    Cgi(LocationBlock const &location);
    ~Cgi();

    /* ******************************** */
    /* *********** make body ********** */
    /* ******************************** */
    std::string         makeBodyCgi(void);

private:
    /* ******************************** */
    /* ************ initial *********** */
    /* ******************************** */
    void                initialPipe(void);
    void                initialEnviron(void);
};

} // namespace ft

#endif