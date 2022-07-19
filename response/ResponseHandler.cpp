#include "ResponseHandler.hpp"
#include "dirent.h"
ResponseHandler::ResponseHandler()
{
}
ResponseHandler::~ResponseHandler()
{
}

//Last_modified 시간 가져오는 함수 
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
// 리퀘스트에서 파싱한 파일경로가 유효한지 검사... 
// 유효하지 못하면 404 에러 ...

void	ResponseHandler::checkFilePath()
{
	Response response;
	
//	this->filePath_ = "/Users/eyoo/web/response"; /*request에서 파싱한  파일 경로 넣어준다. */ 
	struct stat buf; /* stat 함수 (파일 경로 유효성 검사를 위한 함수 구조체)*/
	std::string indexPath;
//	std::string index = "index.php";  /*Config 인덱스파일을 넣어준다.*/
//	int autoindex = 0; /*Config 오토인덱스 값을 넣어준다. */

	if (stat(this->filePath_.c_str(), &buf) != 0) /*경로를 찾지못하면 에러처리 */
	{
		return (response.map_make_pair("404"));
	}
	if (S_ISREG (buf.st_mode)) /*경로가 파일이라면... */
	{
	//	std::cout << "regular file" << std::endl;
		std::string last_modified = getLastTime(buf.st_mtime);
		response.appendHeader("Last-modified", last_modified);
		response.appendHeader("Content-Length", ft_itos(buf.st_size));
	//	response.response_manager(); 결과값 프린트 
		return ;
	}
	if (autoindex) /*오토인덱스가 "ON" 이라면 */
		return ;
	indexPath = filePath_ + "/" + index; /*경로가 디렉토리라면...인덱스파일을 연결... */
	
	if (stat(indexPath.c_str(), &buf) != 0) /*경로 유효성 검사*/ 
		return (response.map_make_pair("404"));

	if (S_ISDIR (buf.st_mode)) /*마지막 체크 또 경로가 디렉토리라면...404에러*/
	{
	//	std::cout << "directory file" << std::endl;
		return (response.map_make_pair("404"));
	}

	this->filePath_ = indexPath;
	response.appendHeader("Content-Length", ft_itos(buf.st_size));
//	response.response_manager(); 결과값 프린트 
}

std::string ResponseHandler::AutoIndex()
{
	struct dirent *current; /*디렉토리또는 파일이 가지고있는 정보 구조체! 파일 이름들 반환예정*/
	DIR *directory; /*Opendir디렉토리스트림을 열고 그 스트림의 포인터값 반환*/
	std::string indexPage; /*만들어질 오토인덱스 페이지 스트링값들*/
	std::string url;  /****  url 값 가져오기*/

	directory = opendir(this->filePath_.c_str());
	indexPage += "<html>\n\t<body>\n\t<h1>Index of </h1>\n";
	while ((current = readdir(directory)) != NULL)
	{
		if (current->d_name[0] != '.')
		{
			indexPage += "\n\t\t\t<a href=\"http://" + url + current->d_name + "\">" + current->d_name + "</a><br>\n</body>\n</html>\n";
		}
	}
	closedir(directory);
	return (indexPage);
}
