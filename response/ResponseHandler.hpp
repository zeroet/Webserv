#ifndef RESPONSEHANDLER_HPP
# define RESPONSEHANDLER_HPP

#include "Response.hpp"
#include <sys/stat.h>

class ResponseHandler
{
	private:

		std::string	_file_path;

	public:
		ResponseHandler();
		ResponseHandler(ResponseHandler const & copy);
		ResponseHandler & operator=(ResponseHandler const & copy);
		~ResponseHandler();

		void	checkFilePath();
		std::string ft_itos(int num);
		std::string getLastTime(time_t time);
};

#endif
		
