#ifndef MIMETYPE_HPP
# define MIMETYPE_HPP

#include <map>
#include "Response.hpp"

class MimeType
{
public:
    typedef std::map<std::string, std::string>              mimeMap;
    typedef std::map<std::string, std::string>::iterator    it_;

private:
    mimeMap     MIMEMap_;

public:
    MimeType();
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

#endif