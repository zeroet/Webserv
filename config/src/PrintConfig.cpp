#include "PrintConfig.hpp"

namespace ft
{
	void	PrintConfig::printBaseDirectivesParameters(ft::BaseDirectives& context)
	{
		std::vector<std::string>		vecString = context.getIndex();
		std::vector<std::string>::iterator	currentString = vecString.begin();
		std::vector<std::string>::iterator	    endString = vecString.end();

		std::cout << "Base root: " << context.getRoot() << std::endl;
		std::cout << "Base client max body size: " << context.getClientMaxBodySize() << std::endl;
		std::cout << "Base keepalive timeout: " << context.getKeepaliveTimeout() << std::endl;
		std::cout << "Base index: ";
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
		std::cout << "Base autoindex: " << context.getAutoindex() << std::endl;
		std::cout << "Base error page: " << context.getErrorPage() << std::endl;
	}

	void	PrintConfig::printHttpContextDirectiveParameters(ft::HttpBlock& context)
	{
		std::cout << ORANGE << "\n[HttpBlock] directives parameters.\n";
		printBaseDirectivesParameters(context);
	}

	void	PrintConfig::printServerContextDirectiveParameters(ft::ServerBlock& context)
	{
		std::vector<std::string>		vecString = context.getServerName();
		std::vector<std::string>::iterator	currentString = vecString.begin();
		std::vector<std::string>::iterator	    endString = vecString.end();

		std::cout << YELLOW << "\n=======================[ServerBlock] directives parameters=====================\n";
		printBaseDirectivesParameters(context);
		std::cout << "listen: " << context.getListen() << std::endl;
		std::cout << "server_name: ";
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
		std::cout << "return: ";
		vecString = context.getReturn();
		currentString = vecString.begin();
		endString = vecString.end();
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << FIN << std::endl;
	}

	void	PrintConfig::printLocationContextDirectiveParameters(ft::LocationBlock& context)
	{
		std::vector<std::string>		vecString = context.getLimitExcept();
		std::vector<std::string>::iterator	currentString = vecString.begin();
		std::vector<std::string>::iterator	    endString = vecString.end();

		std::cout << "\n---------------------[LocationBlock] directives parameters--------------------\n";
		printBaseDirectivesParameters(context);
		std::cout << "limit_except: ";
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
		std::cout << "return: ";
		vecString = context.getReturn();
		currentString = vecString.begin();
		endString = vecString.end();
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
		vecString = context.getCgi();
		std::cout << "cgi: ";
		currentString = vecString.begin();
		endString = vecString.end();
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
		std::cout << "cgi_path: " << context.getCgiPath() << std::endl;
	}

	void	PrintConfig::printConfig(std::pair<bool, HttpBlock>& validHttpContext, int flags)
	{
		if (validHttpContext.first == true)
		{
			printHttpContextDirectiveParameters(validHttpContext.second);
			std::vector<ft::ServerBlock>::iterator	currentServer = validHttpContext.second.serverList.begin();
			std::vector<ft::ServerBlock>::iterator	    endServer = validHttpContext.second.serverList.end();

			for (;currentServer != endServer; ++currentServer)
			{
				printServerContextDirectiveParameters(*currentServer);
				if ((flags & P_LOCATION) == P_LOCATION)
				{
					std::vector<ft::LocationBlock>::iterator	currentLocation = currentServer->locationList.begin();
					std::vector<ft::LocationBlock>::iterator	    endLocation = currentServer->locationList.end();
					for (;currentLocation != endLocation; ++currentLocation)
						printLocationContextDirectiveParameters(*currentLocation);
				}
			}
		}
	}

	void	PrintConfig::printTokens(std::vector<Token>& tokens)
	{
		std::cout << "\t\t <Print tokenized tokens>\n";
		std::cout << "example: Token(token type, text, line number)\n\n";
		std::vector<ft::Token>::iterator	currentToken = tokens.begin();
		std::vector<ft::Token>::iterator	endToken = tokens.end();
		(void)ft::sTokenTypeStrings[currentToken->type];
		for (;currentToken != endToken; ++currentToken)
			currentToken->debugPrint();
	}

	std::pair<bool, HttpBlock>	PrintConfig::parseConfig(char* configPath, int flags)
	{
		std::pair<bool, ft::HttpBlock>	validHttpContext;
		FILE* fp;

		if (configPath == NULL)
			fp = fopen(CONFIG_PATH DEFALUT_CONF, "r");
		else
			fp = fopen(configPath, "r");
		if (!fp)
			std::cerr << "Error: Can not open file.\n";
		else
		{
			fseek(fp, 0, SEEK_END);
			size_t fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			std::string fileContents(fileSize, ' ');
			fread((void*)fileContents.data(), 1, fileSize, fp);

			if ((flags & P_CONTENT) == P_CONTENT)
			{
				std::cout << "\t\t <Print file contents>\n";
				std::cout << fileContents << std::endl; // to print config file contents.
			}
			ft::Tokenizer tokenizer;
			std::vector<ft::Token> tokens = tokenizer.parse(fileContents); // to tokenize file contents.

			if ((flags & P_TOKEN) == P_TOKEN)
				printTokens(tokens); // to print tokenized tokens.

			ft::Parser	parser;
			validHttpContext = parser.parse(tokens);

			if ((flags & P_SERVER) == P_SERVER)
				printConfig(validHttpContext, P_SERVER); // to print server directives
			else if ((flags & P_LOCATION) == P_LOCATION)
				printConfig(validHttpContext, P_LOCATION); // to print location directives
			fclose(fp);
		}
		return (validHttpContext);
	}
};
