#ifndef REQUEST_HPP
# define REQUEST_HPP

# define CR '\r'
# define LF '\n'

# include <string>
# include <vector>
# include <map>

//Parsing buf
class ParseRequest {
	public:
		ParseRequest(void) {}
		ParseRequest(ParseRequest const &src);
		ParseRequest &operator=(ParseRequest const &src);
		~ParseRequest(void);

		// void	SetRequest(RequestComplet &request);

	private:
		std::vector<std::string>			RequestLine_[3];
		std::map<std::string, std::string>	RequestHeader_;
		std::string							RequestBody_;
};