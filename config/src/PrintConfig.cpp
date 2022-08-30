#include "PrintConfig.hpp"

namespace ft
{
	void	PrintConfig::printBaseDirectivesParameters(ft::BaseDirectives& context)
	{
		std::vector<std::string>		vec_string = context.getIndex();
		std::vector<std::string>::iterator	current_string = vec_string.begin();
		std::vector<std::string>::iterator	    end_string = vec_string.end();

		std::cout << "Base root: " << context.getRoot() << std::endl;
		std::cout << "Base client max body size: " << context.getClientMaxBodySize() << std::endl;
		std::cout << "Base keepalive timeout: " << context.getKeepaliveTimeout() << std::endl;
		std::cout << "Base index: ";
		for (; current_string != end_string; ++current_string)
			std::cout << " " << *current_string;
		std::cout << std::endl;
		std::cout << "Base autoindex: " << context.getAutoindex() << std::endl;
		vec_string = context.getErrorPage();
		current_string = vec_string.begin();
		end_string = vec_string.end();
		std::cout << "Base error page: ";// << context.getErrorPage() << std::endl;
		for (; current_string != end_string; ++current_string)
			std::cout << " " << *current_string;
		std::cout << std::endl;
	}

	void	PrintConfig::printHttpContextDirectiveParameters(ft::HttpBlock& context)
	{
		std::cout << ORANGE << "\n[HttpBlock] directives parameters.\n";
		printBaseDirectivesParameters(context);
	}

	void	PrintConfig::printServerContextDirectiveParameters(ft::ServerBlock& context)
	{
		std::vector<std::string>		vec_string = context.getServerName();
		std::vector<std::string>::iterator	current_string = vec_string.begin();
		std::vector<std::string>::iterator	    end_string = vec_string.end();

		std::cout << YELLOW << "\n=======================[ServerBlock] directives parameters=====================\n";
		printBaseDirectivesParameters(context);
		std::cout << "listen: " << context.getListen() << std::endl;
		std::cout << "server_name: ";
		for (; current_string != end_string; ++current_string)
			std::cout << " " << *current_string;
		std::cout << std::endl;
		std::cout << "return: ";
		vec_string = context.getReturn();
		current_string = vec_string.begin();
		end_string = vec_string.end();
		for (; current_string != end_string; ++current_string)
			std::cout << " " << *current_string;
		std::cout << FIN << std::endl;
	}

	void	PrintConfig::printLocationContextDirectiveParameters(ft::LocationBlock& context)
	{
		std::vector<std::string>		vec_string = context.getLimitExcept();
		std::vector<std::string>::iterator	current_string = vec_string.begin();
		std::vector<std::string>::iterator	    end_string = vec_string.end();

		std::cout << "\n---------------------[LocationBlock] directives parameters--------------------\n";
		printBaseDirectivesParameters(context);
		std::cout << "limit_except: ";
		for (; current_string != end_string; ++current_string)
			std::cout << " " << *current_string;
		std::cout << std::endl;
		std::cout << "return: ";
		vec_string = context.getReturn();
		current_string = vec_string.begin();
		end_string = vec_string.end();
		for (; current_string != end_string; ++current_string)
			std::cout << " " << *current_string;
		std::cout << std::endl;
		vec_string = context.getCgi();
		std::cout << "cgi: ";
		current_string = vec_string.begin();
		end_string = vec_string.end();
		for (; current_string != end_string; ++current_string)
			std::cout << " " << *current_string;
		std::cout << std::endl;
		std::cout << "cgi_path: " << context.getCgiPath() << std::endl;
		std::cout << "uri_path: " << context.getUriPath() << std::endl;
	}

	void	PrintConfig::printConfig(std::pair<bool, HttpBlock>& http_pair, int flags)
	{
		if (http_pair.first == true)
		{
			printHttpContextDirectiveParameters(http_pair.second);
			std::vector<ft::ServerBlock>::iterator	currentServer = http_pair.second.server_list.begin();
			std::vector<ft::ServerBlock>::iterator	    endServer = http_pair.second.server_list.end();

			for (;currentServer != endServer; ++currentServer)
			{
				printServerContextDirectiveParameters(*currentServer);
				if ((flags & P_LOCATION) == P_LOCATION)
				{
					std::vector<ft::LocationBlock>::iterator	currentLocation = currentServer->location_list.begin();
					std::vector<ft::LocationBlock>::iterator	    endLocation = currentServer->location_list.end();
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
		std::vector<ft::Token>::iterator	current_token = tokens.begin();
		std::vector<ft::Token>::iterator	end_token = tokens.end();
		(void)ft::sTokenTypeStrings[current_token->type];
		for (;current_token != end_token; ++current_token)
			current_token->debugPrint();
	}

	std::pair<bool, HttpBlock>	PrintConfig::parseConfig(char* config_path, int flags)
	{
		std::pair<bool, ft::HttpBlock>	http_pair;
		FILE* fp;

		if (config_path == NULL)
			fp = fopen(CONFIG_PATH DEFALUT_CONF, "r");
		else
			fp = fopen(config_path, "r");
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
			http_pair = parser.parse(tokens);

			if ((flags & P_SERVER) == P_SERVER)
				printConfig(http_pair, P_SERVER); // to print server directives
			else if ((flags & P_LOCATION) == P_LOCATION)
				printConfig(http_pair, P_LOCATION); // to print location directives
			fclose(fp);
		}
		return (http_pair);
	}
};
