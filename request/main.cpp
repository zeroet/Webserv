#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
			std::cout << line << std::endl;
		// std::cout << "parse" << std::endl;
	}
	return (0);
}