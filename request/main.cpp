#include <iostream>
#include "Request.hpp"

int	main(int ac, char** av) {
	(void)av;
	if (ac != 2)
	{
		std::cout << "parse exemple needed" << std::endl;
		return (1);
	}
	else
	{
		std::cout << "parse" << std::endl;
	}
	return (0);
}