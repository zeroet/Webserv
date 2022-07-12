#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <istream>
#include "Request.hpp"

int	main(int ac, char** av) {
	if (ac != 2)
	{
		std::cout << "parse exemple needed" << std::endl;
		return (1);
	}
	else
	{
		std::ifstream file(av[1]);
		std::string line;
		while (std::getline(file, line))
		{
			std::cout << line << std::endl;
			if (line.find("\r\n\r\n") != (std::string::npos))
				std::cout << "fin" << std::endl;
		}
	}
	return (0);
}