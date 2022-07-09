#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include <map>

class Request {
	public:
		Request(void);
		~Request(void);
	
	private:
		std::vector<std::string> RequestLine_[3];
		std::map<std::string, std::string> RequestHeader_;
		std::string RequestBody_;
};

#endif