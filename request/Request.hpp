#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include <map>

//recevie all until \r\n\r\n
//error
//1: timeout (timestamp)
//2: size > 
class RecvRequest { 
	public:
		RecvRequest(void) {}
		RecvRequest(RecvRequest const &src);
		RecvRequest &operator=(RecvRequest const &src);
		~RecvRequest(void);
	
	private:
		std::string buf;
};

//Parsing buf
class ParseRequest {
	public:
		ParseRequest(void) {}
		ParseRequest(ParseRequest const &src);
		ParseRequest &operator=(ParseRequest const &src);
		~ParseRequest(void);
		
		void	SetRequest(RequestComplet &request);

	private:
		std::vector<std::string>			RequestLine_[3];
		std::map<std::string, std::string>	RequestHeader_;
		std::string							RequestBody_;
};

//Class for Response
class RequestComplet {
	public:
		RequestComplet(void) {}
		RequestComplet(RequestComplet const &src);
		RequestComplet &operator=(RequestComplet const &src);
		~RequestComplet(void);
	
	private:
		std::string							Method_;
		std::string							Path_;
		std::string							Version_;
		std::map<std::string, std::string>	RequestHeader_;
		std::string							RequestBody_;
};

#endif