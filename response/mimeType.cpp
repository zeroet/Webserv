#include "mimeType.hpp"

namespace ft
{

/* ******************************* */
/* ** constructor / destructor *** */
/* ******************************* */
MimeType::MimeType() {
    setMIMETypeTable();
}

MimeType::MimeType(MimeType const &copy) {
    (*this) = copy;
}

MimeType &MimeType::operator=(MimeType const &x) {
    MIMEMap_ = x.MIMEMap_;
    return (*this);

}

MimeType::~MimeType() {
}





/* ******************************* */
/* ************ getter  ********** */
/* ******************************* */
std::string MimeType::getMIMEType(std::string const &Ext)
{

    it_ itFind(this->MIMEMap_.find(Ext));
    if (itFind != MIMEMap_.end())
    {
        return itFind->second;
    }
    return ("text/html");
}






/* ****************************** */
/* *********** setter *********** */
/* ****************************** */
/*
 * https://docs.w3cub.com/http/basics_of_http/mime_types/complete_list_of_mime_types.html
 */
void MimeType::setMIMETypeTable(void)
{
    this->MIMEMap_.insert(std::make_pair("aac", "audio/aac"));
    this->MIMEMap_.insert(std::make_pair("abw", "application/x-abiword"));
    this->MIMEMap_.insert(std::make_pair("arc", "application/x-freearc"));
    this->MIMEMap_.insert(std::make_pair("avi", "video/x-msvideo"));
    this->MIMEMap_.insert(std::make_pair("bin", "application/octet-stream"));
    this->MIMEMap_.insert(std::make_pair("bmp", "image/bmp"));
    this->MIMEMap_.insert(std::make_pair("bz2", "application/x-bzip2"));
    this->MIMEMap_.insert(std::make_pair("csh", "application/x-csh"));
    this->MIMEMap_.insert(std::make_pair("css", "text/css"));
    this->MIMEMap_.insert(std::make_pair("csv", "text/csv"));
    this->MIMEMap_.insert(std::make_pair("doc", "application/msword"));
    this->MIMEMap_.insert(std::make_pair("gif", "image/gif"));
    this->MIMEMap_.insert(std::make_pair("htm", "text/html"));
    this->MIMEMap_.insert(std::make_pair("html", "text/html"));
    this->MIMEMap_.insert(std::make_pair("jpeg", "image/jpeg"));
    this->MIMEMap_.insert(std::make_pair("jpg", "image/jpeg"));
    this->MIMEMap_.insert(std::make_pair("mpeg", "video/mpeg"));
    this->MIMEMap_.insert(std::make_pair("png", "image/png"));
    this->MIMEMap_.insert(std::make_pair("php", "application/x-httpd-php"));
    this->MIMEMap_.insert(std::make_pair("pdf", "application/pdf"));
    this->MIMEMap_.insert(std::make_pair("txt", "text/plain"));
}

} // namespace ft