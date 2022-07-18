#include "ResponseHandler.hpp"

ResponseHandler::ResponseHandler()
{
}
ResponseHandler::~ResponseHandler()
{
}

std::string ResponseHandler::getLastTime(time_t time)
{
	char s[200];
	struct tm *get_time = std::gmtime(&time);

	strftime(s, sizeof(s), "%a, %d %b %Y %T GMT", get_time);
	return (s);
}

std::string ResponseHandler::ft_itos(int num)
{
	std::string str;
	long long	nbr = (long long)num;

	if (num == 0)
		return ("0");
	if (num < 0)
		nbr *= -1;
	while (nbr != 0)
	{
		str = static_cast<char>((nbr % 10) + 48) + str;
		nbr /= 10;
	}
	if (num < 0)
		str = "-" + str;
	return (str);
}
// 리퀘스트에서 구성한 파일경로가 유효한지 검사... 
// 유효하지 못하면 404에 ...

void	ResponseHandler::checkFilePath()
{
	Response response;
	this->_file_path = "/Users/eyoo/web_seo/includes/Client.hpp";
	//request 파일 경로 가져와야함! 
	struct stat buf; // 유효성 검사....

	if (stat(this->_file_path.c_str(), &buf) != 0)//경로를 찾지못하면..
	{
		return (response.map_make_pair("404"));
	}
	if (S_ISREG (buf.st_mode))//경로가 파일이라면 
	{
	//	std::cout << "regular file" << std::endl;
		std::string last_modified = getLastTime(buf.st_mtime);
		response.appendHeader("Last-modified", last_modified);
		response.appendHeader("Content-Length", ft_itos(buf.st_size));
		response.response_manager();
		return ;
	}

	if (S_ISDIR (buf.st_mode))//경로가 디렉토리라면 
	{
		std::cout << "directory file" << std::endl;
		return (response.map_make_pair("404"));
	}

}

