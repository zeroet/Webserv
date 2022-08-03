#ifndef PRINTCONFIG_HPP
#define PRINTCONFIG_HPP
#include "Parser.hpp"
#include <bits/stdc++.h>

#define CONFIG_PATH "./conf/"
#define DEFALUT_CONF "sample.conf"
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

namespace ft
{

	class PrintConfig
	{
		public:
			static void				printBaseDirectivesParameters(BaseDirectives& context);
			static void				printHttpContextDirectiveParameters(HttpBlock& context);
			static void				printServerContextDirectiveParameters(ServerBlock& context);
			static void				printLocationContextDirectiveParameters(LocationBlock& context);
			static void				printConfig(std::pair<bool, HttpBlock>& validHttpContext, int flags);
			static void				printTokens(std::vector<Token>& tokens);
			static std::pair<bool, HttpBlock>	parseConfig(char* configPath, int flags = 0);
	};
}
#endif
