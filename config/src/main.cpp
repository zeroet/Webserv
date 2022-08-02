#include <iostream>
#include <string>
#include <vector>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "BaseDirectives.hpp"
#include "HttpBlock.hpp"
#include "Directive.hpp"
#include <cstdio>

namespace ft
{
	void	printBaseDirectivesParameters(ft::BaseDirectives& context)
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

	void	printHttpContextDirectiveParameters(ft::HttpBlock& context)
	{
		std::cout << "\n[HttpBlock] directives parameters.\n";
		printBaseDirectivesParameters(context);
	}

	void	printServerContextDirectiveParameters(ft::ServerBlock& context)
	{
		std::vector<std::string>		vecString = context.getServerName();
		std::vector<std::string>::iterator	currentString = vecString.begin();
		std::vector<std::string>::iterator	    endString = vecString.end();

		std::cout << "\n=======================[ServerBlock] directives parameters=====================\n";
		printBaseDirectivesParameters(context);
		std::cout << "Base listen: " << context.getListen() << std::endl;
		std::cout << "Base server_name: ";
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
		std::cout << "Base return: ";
		vecString = context.getReturn();
		currentString = vecString.begin();
		endString = vecString.end();
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
	}

	void	printLocationContextDirectiveParameters(ft::LocationBlock& context)
	{
		std::vector<std::string>		vecString = context.getLimitExcept();
		std::vector<std::string>::iterator	currentString = vecString.begin();
		std::vector<std::string>::iterator	    endString = vecString.end();

		std::cout << "\n---------------------[LocationBlock] directives parameters--------------------\n";
		printBaseDirectivesParameters(context);
		std::cout << "Base limit_except: ";
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
		std::cout << "Base return: ";
		vecString = context.getReturn();
		currentString = vecString.begin();
		endString = vecString.end();
		for (; currentString != endString; ++currentString)
			std::cout << " " << *currentString;
		std::cout << std::endl;
	}
};

int main(int argc, char** argv)
{
	(void)argc;
	FILE* fh = fopen(argv[1], "r");
	if (!fh)
		std::cerr << "can not open file,\n";
	fseek(fh, 0, SEEK_END);
	size_t fileSize = ftell(fh);
	fseek(fh, 0, SEEK_SET);
	std::string fileContents(fileSize, ' ');
	fread((void*)fileContents.data(), 1, fileSize, fh);

	//std::cout << fileContents << std::endl;
	ft::Tokenizer tokenizer;
	std::vector<ft::Token> tokens = tokenizer.parse(fileContents);
	
	std::vector<ft::Token>::iterator	currentToken = tokens.begin();
	//std::vector<ft::Token>::iterator	endToken = tokens.end();

	//for (ft::Token currToken : tokens)

	//std::cout << ft::sTokenTypeStrings[currentToken->type] << std::endl; // for debuging
	(void)ft::sTokenTypeStrings[currentToken->type];
		//currToken.debugPrint();
	ft::Parser	parser;
	//ft::HttpBlock	httpContext;
	std::pair<bool, ft::HttpBlock>	validHttpContext;
	validHttpContext = parser.parse(tokens);
	//for (;currentToken != endToken; ++currentToken)
	//	currentToken->debugPrint();

	//std::cout << validHttpContext.first << std::endl;
	if (validHttpContext.first == true)
	{
		printHttpContextDirectiveParameters(validHttpContext.second);
		std::vector<ft::ServerBlock>::iterator	currentServer = validHttpContext.second.serverList.begin();
		std::vector<ft::ServerBlock>::iterator	    endServer = validHttpContext.second.serverList.end();

		for (;currentServer != endServer; ++currentServer)
		{
			printServerContextDirectiveParameters(*currentServer);
			std::vector<ft::LocationBlock>::iterator	currentLocation = currentServer->locationList.begin();
			std::vector<ft::LocationBlock>::iterator	    endLocation = currentServer->locationList.end();
			for (;currentLocation != endLocation; ++currentLocation)
				printLocationContextDirectiveParameters(*currentLocation);
		}
	}
	fclose(fh);
	return (0);
}
