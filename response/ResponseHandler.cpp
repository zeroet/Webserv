#include "ResponseHandler.hpp"
#include "dirent.h"
#include <unistd.h>

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
	
	this->filePath_ = "/Users/eyoo/web/response"; /*request에서 파싱한  파일 경로 넣어준다. */ 
	struct stat buf; /* stat 함수 (파일 경로 유효성 검사를 위한 함수 구조체)*/
	std::string indexPath;
	std::string index = "index.php";  /*Config 인덱스파일을 넣어준다.*/
	int autoindex = 0; /*Config 오토인덱스 값을 넣어준다. */

	if (stat(this->filePath_.c_str(), &buf) != 0) /*경로를 찾지못하면 에러처리 */
	{
		std::cout << "경로를 못찾음" << std::endl;
		return (response.MapStatusCode("404"));
	}
	if (S_ISREG (buf.st_mode)) /*경로가 파일이라면... */
	{
		std::cout << "regular file" << std::endl;
		std::string last_modified = getLastTime(buf.st_mtime);
		response.appendHeader("Last-modified", last_modified);
		response.appendHeader("Content-Length", ft_itos(buf.st_size));
		response.response_manager(); //결과값 프린트 
		return ;
	}
	if (autoindex) /*오토인덱스가 "ON" 이라면 */
		return ;
	indexPath = filePath_ + "/" + index; /*경로가 디렉토리라면...인덱스파일을 연결... */
	
	if (stat(indexPath.c_str(), &buf) != 0) /*경로 유효성 검사*/ 
		return (response.MapStatusCode("404"));

	if (S_ISDIR (buf.st_mode)) /*마지막 체크 또 경로가 디렉토리라면...404에러*/
	{
		std::cout << "directory file" << std::endl;
		return (response.MapStatusCode("404"));
	}

	this->filePath_ = indexPath;
	response.appendHeader("Content-Length", ft_itos(buf.st_size));
	response.response_manager(); //결과값 프린트 
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

void ResponseHandler::DeleteMethod()
{
	struct stat buf;
	Response response;

	if (stat(this->filePath_.c_str(), &buf) == 0) /*경로가 올바르다면 */
	{
		if (S_ISDIR(buf.st_mode))/*경로가 디렉토리라면 베드리퀘스트*/
			return (response.MapStatusCode("400"));
		if (!S_ISREG(buf.st_mode))/*경로가 파일이아니라면 베드리퀘스트 */
			return (response.MapStatusCode("400"));
	}
	response.setStatusCode("204"); /*No content 코드 부여*/
	unlink(this->filePath_.c_str()); /*파일 언링크 */

	std::cout << "delete completed" << std::endl;
}


///PUT METHOD 안한다고 해서 주석처리함(혹시 몰라서 나중에 쓸지도...)  
//나중에 쓰려면 MakeContentLocation 함수 만들어야함... 
/*void ResponseHandler::PutMethod()
{
	int	fd;
	struct stat buf;
	Response response;

	if (stat(this->filePath_.c_str(), &buf) == 0)
	{
		if (S_ISREG(buf.st_mode))
		{
			fd = open(filePath_.c_str(), O_WRONLY | O_TRUNC, 0666);
			//파일을 쓰기 전용으로 엽니다.파일이 이미 존재한다면 이미 존재하는 파일의 내용을 무시하여 엽니다.  모두 읽기, 쓰기 권한을 가진 파일
			if (fd == -1)// 파일이 없다면 에러  forbidden
				return (response.MapStatusCode("403"));
			response.statusCode_ = 204;
		}
		else
		{
			if (S_ISDIR(buf.st_mode))
				return (response.MapStatusCode("400"));
			fd = open(filePath_.c_str(), O_WRONLY | O_TRUNC, 0666);
			if (fd == -1)
				return (response.MapStatusCode("503"));
			response.statusCode_ = 201;
			response.appendHeader("Content-Length", "0");
		}
		std::string content_location = this->makeContentLocation();
		response.appendHeader("Location", content_location);
		response.appendHeader("Content-Location", content_location);
	}
*/





