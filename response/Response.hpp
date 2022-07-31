#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>

class Response
{
	public:
		Response();
//		Response(const std::string &status);
		Response(Response const & copy);
		Response & operator =(Response const & copy);
		~Response();
	
		typedef std::map<std::string, std::string> mapStatus;	
		
		std::string 	makeHeader();
		void			appendHeader(std::string first, std::string second);
		void			setBody(std::string & str);
		void			makeStatusBody();
		void			makeStatusBody(std::string url);
		int				getBodySize();
		void			MapStatusCode(std::string code);
		void			response_manager();
		std::string		getStatusCode();
		void			setStatusCode(std::string code);
	
	private:
		std::map<std::string, std::string> Headers_;
		std::string StatusCode_;
		std::string StatusMessage_;
		std::string Body_;
		mapStatus mapStatus_;

		
	};

#endif
