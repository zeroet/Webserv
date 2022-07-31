#ifndef RESPONSEHANDLER_HPP
# define RESPONSEHANDLER_HPP

#include "Response.hpp"
#include <sys/stat.h>
#include <time.h>

class ResponseHandler
{
	private:

		std::string	filePath_;

	public:
		ResponseHandler();
		ResponseHandler(ResponseHandler const & copy);
		ResponseHandler & operator=(ResponseHandler const & copy);
		~ResponseHandler();

		void	checkFilePath();
		void	DeleteMethod();
		std::string ft_itos(int num);
		std::string getLastTime(time_t time);
		std::string	AutoIndex();
		
};

#endif
		
