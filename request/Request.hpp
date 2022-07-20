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
		Request(void);
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
		// void				setRequest(Request *req);
		void				setMethod(std::string method);
		void				setPath(std::string path);
		void				setVersion(std::string version);
		void				setRequestBody(std::string requestbody);

		Request	const		&getRequest(void)	const;
		std::string	const	&getMethod(void)	const;
		std::string const	&getPath(void)	const;
		std::string	const	&getVersion(void)	const;
		std::map<std::string, std::string>	&getRequestHeader(void);
		std::string	const	&getRequestBody(void)	const;

		//example for response (200 OK)
		void	GET1Req(Request *req);
		void	GET2Req(Request *req);
		void	GET3Req(Request *req);
		void	PUTReq(Request *req);
		void	POST1Req(Request *req);
		void	POST2Req(Request *req);
		void	DELETEReq(Request *req);
};

#endif
