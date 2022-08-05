#include <iostream>
#include <string>
#include <vector>
#include "HttpBlock.hpp"
#include "PrintConfig.hpp"

int main(int argc, char** argv)
{
	std::pair<bool, ft::HttpBlock>	httpContext;

	(void)ft::sTokenTypeStrings;
	(void)argc;
	httpContext = ft::PrintConfig::parseConfig(argv[1], P_SERVER); // possible flags: P_TOKEN, P_CONTENT, P_SERVER, P_LOCATION
	return (0);
}
