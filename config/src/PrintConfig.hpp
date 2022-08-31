#ifndef PRINTCONFIG_HPP
#define PRINTCONFIG_HPP
#include "Parser.hpp"
#include <bits/stdc++.h>

#ifndef P_SERVER
# define P_SERVER	   0b01
#endif
#ifndef P_LOCATION
# define P_LOCATION	   0b10
#endif
#ifndef P_TOKEN
# define P_TOKEN	   0b100	
#endif
#ifndef P_CONTENT
# define P_CONTENT	   0b1000	
#endif

# define YELLOW "\033[0;38;5;220m"
# define GREEN "\033[0;38;5;42m"
# define RED "\033[0;38;5;196m"
# define BLUE "\033[0;38;5;21m"
# define PINK "\033[0;38;5;205m"
# define PURPLE "\033[0;38;5;93m"
# define ORANGE "\033[38;5;202m"
# define FIN "\033[0m"

namespace ft
{

	class PrintConfig
	{
		public:
			static void				printBaseDirectivesParameters(BaseDirectives& context);
			static void				printHttpContextDirectiveParameters(HttpBlock& context);
			static void				printServerContextDirectiveParameters(ServerBlock& context);
			static void				printLocationContextDirectiveParameters(LocationBlock& context);
			static void				printConfig(std::pair<bool, HttpBlock>& http_pair, int flags);
			static void				printTokens(std::vector<Token>& tokens);
			static std::pair<bool, HttpBlock>	parseConfig(char* config_path, int flags = 0);
	};
}
#endif
