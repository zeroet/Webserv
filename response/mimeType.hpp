#ifndef MIMETYPE_HPP
# define MIMETYPE_HPP

#include <string>
#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include <sstream>

namespace ft
{

class MimeType
{
public:
    typedef std::map<std::string, std::string>              mimeMap;
    typedef std::map<std::string, std::string>::iterator    it_;

private:
    mimeMap     MIMEMap_;

public:
    /* ******************************** */
    /* *** constructor / destructor *** */
    /* ******************************** */
    MimeType();
    MimeType(MimeType const &copy);
    MimeType &operator=(MimeType const &x);
    ~MimeType();
    
    /* ******************************* */
    /* ************ getter  ********** */
    /* ******************************* */
    std::string     getMIMEType(std::string const &Ext);

private:
    /* ******************************* */
    /* ************ setter  ********** */
    /* ******************************* */
    void        setMIMETypeTable(void);
};

}
#endif