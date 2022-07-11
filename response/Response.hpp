#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>

class Response
{
	private:
		std::map<std::string, std::string> mHeaders;
		std::string status_code;
		std::string status_message;
		std::string body;
	
	public:
		Response();
		Response(const std::string &status);
		~Response();
		
		
		std::string 	makeHeader();
		void			appendHeader(std::string first, std::string second);
		void			setBody(std::string & str);
		void			makeStatusBody();
		void			makeStatusBody(std::string url);
		int				getBodySize();

		// void	response_manager();
	};

#endif
