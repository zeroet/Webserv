#include "Parser.hpp"

namespace ft
{
	Parser::Parser()
	{
		directives_["http"] = Directive(HTTP, "http");
		directives_["server"] = Directive(SERVER, "server");
		directives_["location"] = Directive(LOCATION, "location");
		directives_["client_max_body_size"] = Directive(CLIENT_MAX_BODY_SIZE, "client_max_body_size");
		directives_["keepalive_timeout"] = Directive(KEEPALIVE_TIMEOUT, "keepalive_timeout");
		directives_["index"] = Directive(INDEX, "index");
		directives_["autoindex"] = Directive(AUTOINDEX, "autoindex");
		directives_["root"] = Directive(ROOT, "root");
		directives_["error_page"] = Directive(ERROR_PAGE, "error_page");
		directives_["listen"] = Directive(LISTEN, "listen");
		directives_["server_name"] = Directive(SERVER_NAME, "server_name");
		directives_["return"] = Directive(RETURN, "return");
		directives_["limit_except"] = Directive(LIMIT_EXCEPT, "limit_except");
	}

	std::pair<bool, HttpBlock>	Parser::parse(std::vector<Token>& tokens)
	{
		modifyIdentifierToken(tokens);
		currentToken_ = tokens.begin();
		startToken_ = tokens.begin();
		endToken_ = tokens.end();
		std::pair<bool, HttpBlock> possibleHttpContext;
		std::pair<bool, Directive> possibleValidDirective;

		while (currentToken_ != endToken_)
		{
			possibleValidDirective = expectHttpContext();
			if (possibleValidDirective.first == false)
			{
				if (expectToken(OPERATOR, "{").first == false)
				{
					//std::cout << "Error: Http context can't have any parameter.\n";
					return (possibleHttpContext);
				}
				else
					++currentToken_;
			}
			else
			{
				possibleHttpContext = parseHttpContext(possibleValidDirective);
				if (possibleHttpContext.first == false)
					return (possibleHttpContext);
			}
		}
		return (possibleHttpContext);
	}

	void	Parser::modifyIdentifierToken(std::vector<Token>& tokens)
	{
		currentToken_ = tokens.begin();
		endToken_ = tokens.end();

		for (;currentToken_ != endToken_; ++currentToken_)
		{
			if (currentToken_->type == IDENTIFIER)
			{
				int i = 0, foundDirectiveKind = -1;
				while (i < LAST_DIRECTIVE_KIND + 1)
				{
					foundDirectiveKind = sDirectiveKindStrings[i].compare(currentToken_->text);
					if (foundDirectiveKind == 0)
					{
						currentToken_->type = DIRECTIVE;
						break ;
					}
					i++;
				}
				if (i == LAST_DIRECTIVE_KIND + 1)
					currentToken_->type = PARAMETER;
			}

		}
	}

	std::pair<bool, Directive>	Parser::checkValidDirective()
	{
		std::pair<bool, Token> possibleDirectiveToken = expectToken(DIRECTIVE);
		std::map<std::string, Directive>::iterator foundDirective;

		if (!possibleDirectiveToken.first)
			return (std::make_pair(false, directives_.begin()->second)); 
		foundDirective = directives_.find(possibleDirectiveToken.second.text);
		if (foundDirective == directives_.end())
		{
			--currentToken_;
			return (std::make_pair(false, directives_.begin()->second)); 
		}
		return (std::make_pair(true, foundDirective->second)); 
	}

	std::pair<bool, Token>	Parser::expectToken(enum TokenType type, const std::string& name)
	{
		Token returnToken;

		if (currentToken_ == endToken_)
			return (std::make_pair(false, returnToken));
		if (currentToken_->type != type)
			return (std::make_pair(false, returnToken));
		if (!name.empty() && currentToken_->text != name)
			return (std::make_pair(false, returnToken));

		returnToken = *currentToken_;
		++currentToken_;
		return (std::make_pair(true, returnToken));
	}

	bool	Parser::setServerDirectiveParameter(ServerBlock& context, std::vector<Directive> directiveList)
	{
		std::vector<Directive>::iterator	currentDirective = directiveList.begin();
		std::vector<Directive>::iterator	endDirective = directiveList.end();

		for (; currentDirective != endDirective; ++currentDirective)
		{
			setBaseDirectiveParameter(context, currentDirective);
			if (currentDirective->directive == LISTEN)
			{
				context.setListen(*currentDirective->parameters.begin());
			}
			else if (currentDirective->directive == SERVER_NAME)
			{
				std::vector<std::string>::iterator	currentParameter = currentDirective->parameters.begin();
				std::vector<std::string>::iterator	    endParameter = currentDirective->parameters.end();

				context.clearServerName();
				for (; currentParameter != endParameter; ++currentParameter)
				{
					context.setServerName(*currentParameter);
				}
			}
			else if (currentDirective->directive == RETURN)
			{
				std::vector<std::string>::iterator	currentParameter = currentDirective->parameters.begin();
				std::vector<std::string>::iterator	    endParameter = currentDirective->parameters.end();
				for (; currentParameter != endParameter; ++currentParameter)
					context.setReturn(*currentParameter);
				// return error code error handling
			}
		}
		return (true);
	}

	bool	Parser::setLocationDirectiveParameter(LocationBlock& context, std::vector<Directive> directiveList)
	{
		std::vector<Directive>::iterator	currentDirective = directiveList.begin();
		std::vector<Directive>::iterator	endDirective = directiveList.end();

		for (; currentDirective != endDirective; ++currentDirective)
		{
			setBaseDirectiveParameter(context, currentDirective);
			if (currentDirective->directive == RETURN)
			{
				std::vector<std::string>::iterator	currentParameter = currentDirective->parameters.begin();
				std::vector<std::string>::iterator	    endParameter = currentDirective->parameters.end();
				for (; currentParameter != endParameter; ++currentParameter)
					context.setReturn(*currentParameter);
				// return error code error handling
			}
			else if (currentDirective->directive == LIMIT_EXCEPT)
			{
				std::vector<std::string>::iterator	currentParameter = currentDirective->parameters.begin();
				std::vector<std::string>::iterator	    endParameter = currentDirective->parameters.end();
				for (; currentParameter != endParameter; ++currentParameter)
					context.setLimitExcept(*currentParameter);
			}
		}
		return (true);
	}

	bool	Parser::setBaseDirectiveParameter(BaseDirectives& context, std::vector<Directive>::iterator& currentDirective)
	{
		if (currentDirective->directive == CLIENT_MAX_BODY_SIZE)
		{
			unsigned long		value;
			std::string		inputString = (*currentDirective->parameters.begin());
			std::string::size_type	n;

			if (inputString.length() != 0)
			{
				n = inputString.find_first_not_of("0123456789");
				if (n == std::string::npos)
				{	
					value = std::strtoul((*currentDirective->parameters.begin()).c_str(), NULL, 10); // string to unsigned long
					context.setClientMaxBodySize(value); 
				}
				else
				{
					std::cout << "Error: client_max_body_size parameter should be an integer literal.\n";
					return (false);
				}

			}
		}
		else if (currentDirective->directive == KEEPALIVE_TIMEOUT)
		{
			unsigned int		value;
			std::string		inputString = (*currentDirective->parameters.begin());
			std::string::size_type	n;

			if (inputString.length() != 0)
			{
				n = inputString.find_first_not_of("0123456789");
				if (n == std::string::npos)
				{	
					std::istringstream(*currentDirective->parameters.begin()) >> value; // string to unsigned int
					context.setKeepaliveTimeout(value); 
				}
				else
				{
					std::cout << "Error: keepalive_timeout parameter should be an integer literal.\n";
					return (false);
				}

			}

		}
		else if (currentDirective->directive == AUTOINDEX)
		{
			if ((*currentDirective->parameters.begin()) == "on")
				context.setAutoindex(true);
			else if ((*currentDirective->parameters.begin()) == "off")
				context.setAutoindex(false);
			else
			{
				std::cout << "Error: autoindex parameter should be either 'on' or 'off'.\n";
				return (false);
			}
		}
		else if (currentDirective->directive == ROOT)
			context.setRoot((*currentDirective->parameters.begin()));
		else if (currentDirective->directive == ERROR_PAGE)
			context.setErrorPage(*(currentDirective->parameters.begin()));
		else if (currentDirective->directive == INDEX)
		{
			std::vector<std::string>::iterator	currentParameter = currentDirective->parameters.begin();
			std::vector<std::string>::iterator	    endParameter = currentDirective->parameters.end();

			context.clearIndex();
			for (; currentParameter != endParameter; ++currentParameter)
				context.setIndex(*currentParameter);
		}
		return (true);
	}

	bool	Parser::setHttpDirectiveParameter(HttpBlock& context, std::vector<Directive> directiveList)
	{
		std::vector<Directive>::iterator	currentDirective = directiveList.begin();
		std::vector<Directive>::iterator	endDirective = directiveList.end();

		for (; currentDirective != endDirective; ++currentDirective)
			setBaseDirectiveParameter(context, currentDirective);
		return (true);
	}

	std::pair<bool, Directive>	Parser::expectHttpContext()
	{
		std::vector<Token>::iterator parseStart = currentToken_;
		std::pair<bool, Directive> possibleValidDirective = checkValidDirective();

		if (currentToken_ == startToken_ && possibleValidDirective.first == false) 
		{
			currentToken_ = parseStart;
			//std::cout << "(start token) = " << startToken_->text << "\n";
			//std::cout << "(current token, line) = " << currentToken_->text << ", " << currentToken_->lineNumber << "\n";
			std::cout << "Error: first directive should be http block.\n";
			return (std::make_pair(false, possibleValidDirective.second));
		}
		if (possibleValidDirective.first == false) 
		{
			currentToken_ = parseStart;
			//std::cout << "(current token, line) = " << currentToken_->text << ", " << currentToken_->lineNumber << "\n";
			//std::cout << "Error: first directive should be http block.\n";
			return (std::make_pair(false, possibleValidDirective.second));
		}
		if (possibleValidDirective.second.directive != HTTP)
		{
			currentToken_ = parseStart;
			std::cout << "Error: It should start with http context but the directive is " << possibleValidDirective.second.name << "\n";
			return (std::make_pair(false, possibleValidDirective.second));
		}
		if (expectToken(OPERATOR, "{").first == false)
		{
			std::cout << "Error: Http context can't have any parameter.\n";
			return (std::make_pair(false, possibleValidDirective.second));
		}
		return (std::make_pair(true, possibleValidDirective.second));
	}

	std::pair<bool, HttpBlock>	Parser::parseHttpContext(std::pair<bool, Directive> validDirective)
	{
		std::vector<Token>::iterator parseStart = currentToken_;
		std::pair<bool, std::vector<Directive> > directives;
		HttpBlock	httpContext;

		while (currentToken_ != endToken_ - 1 && expectToken(OPERATOR, "}").first == false)
		{
			validDirective = expectServerContext();
			if (validDirective.first == false)
			{
				directives = parseContextBody(HTTP);
				if (directives.first == false)
				{
					if (currentToken_ == endToken_ && expectToken(OPERATOR, "}").first == false)
					{
						std::cout << "Error: Http context has not successfuly been enclosed with a closing curly bracket.\n";
						return (std::make_pair(false, httpContext));
					}
					currentToken_ = parseStart;
					return (std::make_pair(false, httpContext));
				}
				if (setHttpDirectiveParameter(httpContext, directives.second) == false)
					return (std::make_pair(false, httpContext));
				
			}
			else
			{
				if (expectToken(OPERATOR, "{").first == false)
				{
					std::cout << "Error: Server block should start with a curly bracket.\n";
					return (std::make_pair(false, httpContext));
				}
				std::pair<bool, ServerBlock>	possibleServerContext = parseServerContext(httpContext, validDirective);

				if (possibleServerContext.first == false)
				{
					currentToken_ = parseStart;
					return (std::make_pair(false, httpContext));
				}
				httpContext.serverList.push_back(possibleServerContext.second);
			}

		}
		return (std::make_pair(true, httpContext));
	}

	std::pair<bool, Directive>	Parser::expectServerContext()
	{
		std::vector<Token>::iterator parseStart = currentToken_;
		std::pair<bool, Directive> possibleValidDirective = checkValidDirective();

		if (possibleValidDirective.first == false) 
		{
			currentToken_ = parseStart;
			return (std::make_pair(false, possibleValidDirective.second));
		}
		if (possibleValidDirective.second.directive != SERVER)
		{
			currentToken_ = parseStart;
			return (std::make_pair(false, possibleValidDirective.second));
		}
		return (std::make_pair(true, possibleValidDirective.second));
	}

	std::pair<bool, ServerBlock>	Parser::parseServerContext(HttpBlock& httpContext, std::pair<bool, Directive> validDirective)
	{
		std::vector<Token>::iterator parseStart = currentToken_;
		std::pair<bool, std::vector<Directive> > directives;
		std::pair<bool, Token> possibleOperator = expectToken(OPERATOR, "}");

		ServerBlock	serverContext(httpContext);
		while (possibleOperator.first == false)
		{
			validDirective = expectLocationContext();
			if (validDirective.first == false &&
				 (validDirective.second.directive == SERVER || validDirective.second.directive == HTTP))
			{
				currentToken_ = parseStart;
				return (std::make_pair(false, serverContext));
			}
			//else if ((validDirective.first == false) &&
					 //(validDirective.second.directive != LOCATION))
			else if (validDirective.first == false)
			{
				directives = parseContextBody(SERVER);
				if (directives.first == false)
				{
					if (expectToken(OPERATOR, "}").first == false)
					{
						std::cout << "Error: Server context has not successfuly been enclosed with a closing curly bracket.\n";
						return (std::make_pair(false, serverContext));
					}
					currentToken_ = parseStart;
					return (std::make_pair(false, serverContext));
				}
				if (setServerDirectiveParameter(serverContext, directives.second) == false)
					return (std::make_pair(false, serverContext));
			}
			else
			{
				if (expectToken(OPERATOR, "{").first == false)
				{
					std::cout << "Error: Multiple parameters, location directive should only have one URI path\n";
					return (std::make_pair(false, serverContext));
				}
				std::pair<bool, LocationBlock>	possibleLocationContext = parseLocationContext(serverContext);
						
				if (possibleLocationContext.first == false)
				{
					currentToken_ = parseStart;
					return (std::make_pair(false, serverContext));
				}
				serverContext.locationList.push_back(possibleLocationContext.second);
			}
			possibleOperator = expectToken(OPERATOR, "}");
		}

		//if (currentToken_ == endToken_ - 1 && expectToken(OPERATOR, "}").first == false)
		return (std::make_pair(true, serverContext));
	}

	std::pair<bool, Directive>	Parser::expectLocationContext()
	{
		std::vector<Token>::iterator parseStart = currentToken_;
		std::pair<bool, Directive> possibleValidDirective = checkValidDirective();

		if (possibleValidDirective.first == false) 
		{
			currentToken_ = parseStart;
			return (std::make_pair(false, possibleValidDirective.second));
		}
		if ((possibleValidDirective.second.directive == SERVER) || 
			(possibleValidDirective.second.directive == HTTP))
		{
			currentToken_ = parseStart;
			std::cout << "Error: There can't be any " << sDirectiveKindStrings[possibleValidDirective.second.directive] 
				<< " block inside a server block" << ", ";
			std::cout << "currentToken is :" << currentToken_->text << "\n";
			return (std::make_pair(false, possibleValidDirective.second));
		}
		if (possibleValidDirective.second.directive != LOCATION)
		{
			currentToken_ = parseStart;
			return (std::make_pair(false, possibleValidDirective.second));
		}
		std::pair<bool, Token> possibleParameter = expectToken(PARAMETER);
		if (possibleParameter.first == false)
		{
			currentToken_ = parseStart;
			std::cout << "Error: No parameter: Location directive should have one URI path\n";
			return (std::make_pair(false, possibleValidDirective.second));
		}
		possibleValidDirective.second.parameters.push_back(possibleParameter.second.text);
		return (std::make_pair(true, possibleValidDirective.second));
	}

	std::pair<bool, LocationBlock>	Parser::parseLocationContext(ServerBlock& serverContext)
	{
		std::vector<Token>::iterator parseStart = currentToken_;
		std::pair<bool, std::vector<Directive> > directives;
		std::pair<bool, Token> possibleOperator;
		LocationBlock	locationContext(serverContext);

		possibleOperator = expectToken(OPERATOR, "}");
		while (possibleOperator.first == false)
		{
			directives = parseContextBody(LOCATION);
			if (directives.first == false)
			{
				if (currentToken_->text != "location" && expectToken(OPERATOR, "}").first == false)
				{
					std::cout << "Error: Location context has not successfuly been enclosed with a closing curly bracket.\n";
					return (std::make_pair(false, locationContext));
				}
				currentToken_ = parseStart;
				return (std::make_pair(false, locationContext));
			}
			if (setLocationDirectiveParameter(locationContext, directives.second) == false)
				return (std::make_pair(false, locationContext));
			possibleOperator = expectToken(OPERATOR, "}");
		}
		return (std::make_pair(true, locationContext));
	}

	std::pair<bool, std::vector<Directive> > Parser::parseContextBody(enum DirectiveKind kind)
	{
		std::vector<Directive>	directives;
		std::pair<bool, Directive> simpleDirective = expectSimpleDirective(kind);

		while (simpleDirective.first == true)
		{
			directives.push_back(simpleDirective.second);
			simpleDirective = expectSimpleDirective(kind);
		}
		if (simpleDirective.first == false) // check if it is a context or an error
		{
			//std::cout << "simpleDirective first false, kind: " << kind << "\n";
			if (kind == HTTP && (simpleDirective.second.directive == SERVER))
			{
				return (std::make_pair(true, directives));
			}
			if (kind == SERVER && (simpleDirective.second.directive == LOCATION))
				return (std::make_pair(true, directives));
			if (kind == SERVER && ((simpleDirective.second.directive == SERVER)))
			{
				std::cout << "Error: There can't be any other block than location in server block.\n";
				return (std::make_pair(false, directives));
			}
			if (kind == LOCATION && ((simpleDirective.second.directive == SERVER) || (simpleDirective.second.directive == HTTP) || (simpleDirective.second.directive == LOCATION)))
			{
				std::cout << "Error: There can't be any other block in location block.\n";
				return (std::make_pair(false, directives));
			}
			if (expectToken(OPERATOR, "}").first == true)
			{
				--currentToken_;
				return (std::make_pair(true, directives));
			}
		}
		return (std::make_pair(false, directives));
	}

	std::pair<bool, Directive>	Parser::expectSimpleDirective(enum DirectiveKind kind)
	{
		//(void)ft::sTokenTypeStrings[currentToken_->type]; // for debuging
		std::vector<Token>::iterator startToken = currentToken_;
		std::pair<bool, Directive> possibleValidDirective = checkValidDirective();

		(void)kind;
		if (possibleValidDirective.first == true)
		{
			if ((possibleValidDirective.second.directive == HTTP) ||
					(possibleValidDirective.second.directive == SERVER) ||
					(possibleValidDirective.second.directive == LOCATION)) // check block directive
			{
				currentToken_ = startToken;
				return (std::make_pair(false, possibleValidDirective.second));
			}
		}
		else if (possibleValidDirective.first == false)
		{
			if (currentToken_ != endToken_ && expectToken(OPERATOR, "}").first == false)
				std::cout << "Error: Unexpected directive, " << currentToken_->text << " in line, " << currentToken_->lineNumber << "\n";
			currentToken_ = startToken;
			return (std::make_pair(false, possibleValidDirective.second));
		}
		return (checkValidParameterNumber(possibleValidDirective));
	}

	std::pair<bool, Directive> Parser::checkValidParameterNumber(std::pair<bool, Directive>& validDirective)
	{
		std::vector<Token>::iterator startToken = currentToken_;
		std::pair<bool, Token> possibleParameter = expectToken(PARAMETER);

		if (possibleParameter.first == true)
		{
			if ((validDirective.second.directive == SERVER_NAME) ||
					(validDirective.second.directive == LIMIT_EXCEPT) ||
					(validDirective.second.directive == INDEX) ||
					(validDirective.second.directive == RETURN))
			{
				while (possibleParameter.first == true)
				{
					validDirective.second.parameters.push_back(possibleParameter.second.text);
					possibleParameter = expectToken(PARAMETER);
				}
			}
			else
				validDirective.second.parameters.push_back(possibleParameter.second.text);
		}
		else
		{
			std::cout << "Error: Simple directive should at least one parameter\n";
			currentToken_ = startToken;
			return (std::make_pair(false, validDirective.second));
		}
		possibleParameter = expectToken(PARAMETER);
		if (possibleParameter.first == true)
		{
			currentToken_ = startToken;
			std::cout << "Error: There can't be more parameters with directive " << validDirective.second.name << ", ";
			std::cout << "additional parameter is " << possibleParameter.second.text<< ".\n";
			return (std::make_pair(false, validDirective.second));
		}
		std::pair<bool, Token> possibleOperator = expectToken(OPERATOR, ";");
		if (possibleOperator.first == false)
		{
			currentToken_ = startToken;
			std::cout << "Error: Simple directive should be terminated by ';' \n";
			return (std::make_pair(false, validDirective.second));
		}
		return (std::make_pair(true, validDirective.second));
	}
}
