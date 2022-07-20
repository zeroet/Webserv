#ifndef REQUEST_HPP
# define REQUEST_HPP

# define CR '\r'
# define LF '\n'

# include <string>
# include <vector>
# include <map>

//Class for Response - Complete Response
class Request {
	public:
		Request(void) {}
		Request(Request const &src);
		Request &operator=(Request const &src);
		~Request(void);

	private:
		std::string							Method_;
		std::string							Path_;
		std::string							Version_;
		std::map<std::string, std::string>	RequestHeader_;
		std::string							RequestBody_;
		// int									ResponseCode_;

	public:
		std::string	const	&getMethod(void)	const;
		std::string const	&getPath(void)	const;
		std::string	const	&getVersion(void)	const;
		std::map<std::string, std::string> const	&getRequestHeader(void)	const;
		std::string	const	&getRequestBody(void)	const;
};

#endif
