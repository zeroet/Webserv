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
		directives_["cgi"] = Directive(CGI, "cgi");
		directives_["cgi_path"] = Directive(CGI_PATH, "cgi_path");
		directives_["limit_except"] = Directive(LIMIT_EXCEPT, "limit_except");
	}

	std::pair<bool, HttpBlock>	Parser::parse(std::vector<Token>& tokens)
	{
		modifyIdentifierToken(tokens);
		current_token_ = tokens.begin();
		start_token_ = tokens.begin();
		end_token_ = tokens.end();
		std::pair<bool, HttpBlock> 	http_pair;
		std::pair<bool, Directive> 	directive_pair;

		while (current_token_ != end_token_)
		{
			directive_pair = expectHttpContext();
			if (directive_pair.first == false)
			{
				if (expectToken(OPERATOR, "{").first == false)
				{
					//std::cout << "Error: Http context can't have any parameter.\n";
					return (http_pair);
				}
				else
					++current_token_;
			}
			else
			{
				http_pair = parseHttpContext(directive_pair);
				if (http_pair.first == false)
					return (http_pair);
			}
		}
		return (http_pair);
	}

	void	Parser::modifyIdentifierToken(std::vector<Token>& tokens)
	{
		current_token_ = tokens.begin();
		end_token_ = tokens.end();

		for (;current_token_ != end_token_; ++current_token_)
		{
			if (current_token_->type == IDENTIFIER)
			{
				int i = 0, found_directive_kind = -1;
				while (i < LAST_DIRECTIVE_KIND + 1)
				{
					found_directive_kind = sDirectiveKindStrings[i].compare(current_token_->text);
					if (found_directive_kind == 0)
					{
						current_token_->type = DIRECTIVE;
						break ;
					}
					i++;
				}
				if (i == LAST_DIRECTIVE_KIND + 1)
					current_token_->type = PARAMETER;
			}

		}
	}

	std::pair<bool, Directive>	Parser::checkValidDirective()
	{
		std::pair<bool, Token> 				token_pair = expectToken(DIRECTIVE);
		std::map<std::string, Directive>::iterator 	found_directive;

		if (!token_pair.first)
			return (std::make_pair(false, directives_.begin()->second)); 
		found_directive = directives_.find(token_pair.second.text);
		if (found_directive == directives_.end())
		{
			--current_token_;
			return (std::make_pair(false, directives_.begin()->second)); 
		}
		return (std::make_pair(true, found_directive->second)); 
	}

	std::pair<bool, Token>	Parser::expectToken(enum TokenType type, const std::string& name)
	{
		Token return_token;

		if (current_token_ == end_token_)
			return (std::make_pair(false, return_token));
		if (current_token_->type != type)
			return (std::make_pair(false, return_token));
		if (!name.empty() && current_token_->text != name)
			return (std::make_pair(false, return_token));

		return_token = *current_token_;
		++current_token_;
		return (std::make_pair(true, return_token));
	}

	bool	Parser::setServerDirectiveParameter(ServerBlock& context, std::vector<Directive> directive_list)
	{
		std::vector<Directive>::iterator	current_directive = directive_list.begin();
		std::vector<Directive>::iterator	end_directive = directive_list.end();

		for (; current_directive != end_directive; ++current_directive)
		{
			if (setBaseDirectiveParameter(context, current_directive) == false)
				return (false);
			if (current_directive->directive == LISTEN)
			{
				unsigned int		value;
				std::string		input_string = (*current_directive->parameters.begin());
				std::string::size_type	n;

				if (input_string.length() != 0)
				{
					n = input_string.find_first_not_of("0123456789");
					if (n == std::string::npos)
					{	
						std::istringstream(*current_directive->parameters.begin()) >> value; // string to unsigned int
						context.setListen(value);
					}
					else
					{
						std::cout << "Error: listen parameter should be an integer literal.\n";
						return (false);
					}

				}
			}
			else if (current_directive->directive == SERVER_NAME)
			{
				std::vector<std::string>::iterator	current_parameter = current_directive->parameters.begin();
				std::vector<std::string>::iterator	    end_parameter = current_directive->parameters.end();

				context.clearServerName();
				for (; current_parameter != end_parameter; ++current_parameter)
				{
					context.setServerName(*current_parameter);
				}
			}
			else if (current_directive->directive == RETURN)
			{
				std::vector<std::string>::iterator	current_parameter = current_directive->parameters.begin();
				std::vector<std::string>::iterator	    end_parameter = current_directive->parameters.end();
				for (; current_parameter != end_parameter; ++current_parameter)
					context.setReturn(*current_parameter);
				// return error code error handling
			}
		}
		return (true);
	}

	bool	Parser::setLocationDirectiveParameter(LocationBlock& context, std::vector<Directive> directive_list)
	{
		std::vector<Directive>::iterator	current_directive = directive_list.begin();
		std::vector<Directive>::iterator	end_directive = directive_list.end();

		for (; current_directive != end_directive; ++current_directive)
		{
			if (setBaseDirectiveParameter(context, current_directive) == false)
				return (false);
			if (current_directive->directive == RETURN)
			{
				std::vector<std::string>::iterator	current_parameter = current_directive->parameters.begin();
				std::vector<std::string>::iterator	    end_parameter = current_directive->parameters.end();
				for (; current_parameter != end_parameter; ++current_parameter)
					context.setReturn(*current_parameter);
				// return error code error handling
			}
			else if (current_directive->directive == LIMIT_EXCEPT)
			{
				std::vector<std::string>::iterator	current_parameter = current_directive->parameters.begin();
				std::vector<std::string>::iterator	    end_parameter = current_directive->parameters.end();
				for (; current_parameter != end_parameter; ++current_parameter)
					context.setLimitExcept(*current_parameter);
			}
			else if (current_directive->directive == CGI)
			{
				std::vector<std::string>::iterator	current_parameter = current_directive->parameters.begin();
				std::vector<std::string>::iterator	    end_parameter = current_directive->parameters.end();
				for (; current_parameter != end_parameter; ++current_parameter)
					context.setCgi(*current_parameter);
			}
			else if (current_directive->directive == CGI_PATH)
			{
				context.setCgiPath((*current_directive->parameters.begin()));
			}
		}
		return (true);
	}

	bool	Parser::setHttpDirectiveParameter(HttpBlock& context, std::vector<Directive> directive_list)
	{
		std::vector<Directive>::iterator	current_directive = directive_list.begin();
		std::vector<Directive>::iterator	end_directive = directive_list.end();

		for (; current_directive != end_directive; ++current_directive)
			if (setBaseDirectiveParameter(context, current_directive) == false)
				return (false);
		return (true);
	}

	bool	Parser::setBaseDirectiveParameter(BaseDirectives& context, std::vector<Directive>::iterator& current_directive)
	{
		if (current_directive->directive == CLIENT_MAX_BODY_SIZE)
		{
			unsigned long		value;
			std::string		input_string = (*current_directive->parameters.begin());
			std::string::size_type	n;

			if (input_string.length() != 0)
			{
				n = input_string.find_first_not_of("0123456789");
				if (n == std::string::npos)
				{	
					value = std::strtoul((*current_directive->parameters.begin()).c_str(), NULL, 10); // string to unsigned long
					context.setClientMaxBodySize(value); 
				}
				else
				{
					std::cout << "Error: client_max_body_size parameter should be an integer literal.\n";
					return (false);
				}

			}
		}
		else if (current_directive->directive == KEEPALIVE_TIMEOUT)
		{
			unsigned int		value;
			std::string		input_string = (*current_directive->parameters.begin());
			std::string::size_type	n;

			if (input_string.length() != 0)
			{
				n = input_string.find_first_not_of("0123456789");
				if (n == std::string::npos)
				{	
					std::istringstream(*current_directive->parameters.begin()) >> value; // string to unsigned int
					context.setKeepaliveTimeout(value); 
				}
				else
				{
					std::cout << "Error: keepalive_timeout parameter should be an integer literal.\n";
					return (false);
				}

			}

		}
		else if (current_directive->directive == AUTOINDEX)
		{
			if ((*current_directive->parameters.begin()) == "on")
				context.setAutoindex(true);
			else if ((*current_directive->parameters.begin()) == "off")
				context.setAutoindex(false);
			else
			{
				std::cout << "Error: autoindex parameter should be either 'on' or 'off'.\n";
				return (false);
			}
		}
		else if (current_directive->directive == ROOT)
			context.setRoot((*current_directive->parameters.begin()));
		else if (current_directive->directive == ERROR_PAGE)
			context.setErrorPage(*(current_directive->parameters.begin()));
		else if (current_directive->directive == INDEX)
		{
			std::vector<std::string>::iterator	current_parameter = current_directive->parameters.begin();
			std::vector<std::string>::iterator	    end_parameter = current_directive->parameters.end();

			context.clearIndex();
			for (; current_parameter != end_parameter; ++current_parameter)
				context.setIndex(*current_parameter);
		}
		return (true);
	}

	std::pair<bool, Directive>	Parser::expectHttpContext()
	{
		std::vector<Token>::iterator parse_start = current_token_;
		std::pair<bool, Directive> directive_pair = checkValidDirective();

		if (current_token_ == start_token_ && directive_pair.first == false) 
		{
			current_token_ = parse_start;
			//std::cout << "(start token) = " << start_token_->text << "\n";
			//std::cout << "(current token, line) = " << current_token_->text << ", " << current_token_->line_num << "\n";
			std::cout << "Error: first directive should be http block.\n";
			return (std::make_pair(false, directive_pair.second));
		}
		if (directive_pair.first == false) 
		{
			current_token_ = parse_start;
			return (std::make_pair(false, directive_pair.second));
		}
		if (directive_pair.second.directive != HTTP)
		{
			current_token_ = parse_start;
			std::cout << "Error: It should start with http context but the directive is " << directive_pair.second.name << "\n";
			return (std::make_pair(false, directive_pair.second));
		}
		if (expectToken(OPERATOR, "{").first == false)
		{
			std::cout << "Error: Http context can't have any parameter.\n";
			return (std::make_pair(false, directive_pair.second));
		}
		return (std::make_pair(true, directive_pair.second));
	}

	std::pair<bool, HttpBlock>	Parser::parseHttpContext(std::pair<bool, Directive> directive_pair)
	{
		std::vector<Token>::iterator 			parse_start = current_token_;
		std::pair<bool, std::vector<Directive> > 	directives;
		HttpBlock					http_context;

		while (current_token_ != end_token_ - 1 && expectToken(OPERATOR, "}").first == false)
		{
			directive_pair = expectServerContext();
			if (directive_pair.first == false)
			{
				directives = parseContextBody(HTTP);
				if (directives.first == false)
				{
					if (current_token_ == end_token_ && expectToken(OPERATOR, "}").first == false)
					{
						std::cout << "Error: Http context has not successfuly been enclosed with a closing curly bracket.\n";
						return (std::make_pair(false, http_context));
					}
					current_token_ = parse_start;
					return (std::make_pair(false, http_context));
				}
				if (setHttpDirectiveParameter(http_context, directives.second) == false)
					return (std::make_pair(false, http_context));
				
			}
			else
			{
				if (expectToken(OPERATOR, "{").first == false)
				{
					std::cout << "Error: Server block should start with a curly bracket.\n";
					return (std::make_pair(false, http_context));
				}
				std::pair<bool, ServerBlock>	server_pair = parseServerContext(http_context, directive_pair);

				if (server_pair.first == false)
				{
					current_token_ = parse_start;
					return (std::make_pair(false, http_context));
				}
				http_context.server_list.push_back(server_pair.second);
			}

		}
		return (std::make_pair(true, http_context));
	}

	std::pair<bool, Directive>	Parser::expectServerContext()
	{
		std::vector<Token>::iterator parse_start = current_token_;
		std::pair<bool, Directive> directive_pair = checkValidDirective();

		if (directive_pair.first == false) 
		{
			current_token_ = parse_start;
			return (std::make_pair(false, directive_pair.second));
		}
		if (directive_pair.second.directive != SERVER)
		{
			current_token_ = parse_start;
			return (std::make_pair(false, directive_pair.second));
		}
		return (std::make_pair(true, directive_pair.second));
	}

	std::pair<bool, ServerBlock>	Parser::parseServerContext(HttpBlock& http_context, std::pair<bool, Directive> directive_pair)
	{
		std::vector<Token>::iterator			parse_start = current_token_;
		std::pair<bool, std::vector<Directive> >	directives;
		std::pair<bool, Token> 				token_pair = expectToken(OPERATOR, "}");
		ServerBlock					server_context(http_context);

		while (token_pair.first == false)
		{
			std::vector<Token>::iterator	location_start = current_token_;
			directive_pair = expectLocationContext();
			if (directive_pair.first == false &&
				 (directive_pair.second.directive == SERVER || 
				 directive_pair.second.directive == HTTP || 
				 directive_pair.second.directive == LOCATION))
			{
				current_token_ = parse_start;
				return (std::make_pair(false, server_context));
			}
			else if (directive_pair.first == false)
			{
				directives = parseContextBody(SERVER);
				if (directives.first == false)
				{
					if (current_token_ == end_token_ && expectToken(OPERATOR, "}").first == false)
					{
						std::cout << "Error: Server context has not successfuly been enclosed with a closing curly bracket.\n";
						return (std::make_pair(false, server_context));
					}
					current_token_ = parse_start;
					return (std::make_pair(false, server_context));
				}
				if (setServerDirectiveParameter(server_context, directives.second) == false)
					return (std::make_pair(false, server_context));
			}
			else
			{
				if (expectToken(OPERATOR, "{").first == false)
				{
					std::cout << "Error: Multiple parameters, location directive should only have one URI path\n";
					return (std::make_pair(false, server_context));
				}
				std::pair<bool, LocationBlock>	location_pair = parseLocationContext(server_context);
						
				if (location_pair.first == false)
				{
					current_token_ = parse_start;
					return (std::make_pair(false, server_context));
				}
				if (server_context.checkLocationUriPath(*directive_pair.second.parameters.begin()) == true)
					location_pair.second.setUriPath(*directive_pair.second.parameters.begin());
				else
				{
					std::cout << "Error: Duplicate location " << *directive_pair.second.parameters.begin();
					std::cout << " in line " << location_start->line_num << "\n";
					return (std::make_pair(false, server_context));
				}
				server_context.location_list.push_back(location_pair.second);
			}
			token_pair = expectToken(OPERATOR, "}");
		}
		return (std::make_pair(true, server_context));
	}

	std::pair<bool, Directive>	Parser::expectLocationContext()
	{
		std::vector<Token>::iterator	parse_start = current_token_;
		std::pair<bool, Directive>	directive_pair = checkValidDirective();

		if (directive_pair.first == false) 
		{
			current_token_ = parse_start;
			return (std::make_pair(false, directive_pair.second));
		}
		if ((directive_pair.second.directive == SERVER) || 
			(directive_pair.second.directive == HTTP))
		{
			current_token_ = parse_start;
			std::cout << "Error: There can't be any " << sDirectiveKindStrings[directive_pair.second.directive] 
				<< " block inside a server block, currentToken is :" << current_token_->text << "\n";
			return (std::make_pair(false, directive_pair.second));
		}
		if (directive_pair.second.directive != LOCATION)
		{
			current_token_ = parse_start;
			return (std::make_pair(false, directive_pair.second));
		}
		std::pair<bool, Token> parameter_token_pair = expectToken(PARAMETER);
		if (parameter_token_pair.first == false)
		{
			current_token_ = parse_start;
			std::cout << "Error: No parameter: Location directive should have one URI path\n";
			return (std::make_pair(false, directive_pair.second));
		}
		directive_pair.second.parameters.push_back(parameter_token_pair.second.text);
		return (std::make_pair(true, directive_pair.second));
	}

	std::pair<bool, LocationBlock>	Parser::parseLocationContext(ServerBlock& server_context)
	{
		std::vector<Token>::iterator 			parse_start = current_token_;
		std::pair<bool, std::vector<Directive> > 	directives;
		std::pair<bool, Token> token_pair;
		LocationBlock	locationContext(server_context);

		token_pair = expectToken(OPERATOR, "}");
		while (token_pair.first == false)
		{
			directives = parseContextBody(LOCATION);
			if (directives.first == false)
			{
				if (current_token_ == end_token_ && expectToken(OPERATOR, "}").first == false)
				//if (current_token_->text != "location" && expectToken(OPERATOR, "}").first == false)
				{
					std::cout << "Error: Location context has not successfuly been enclosed with a closing curly bracket.\n";
					return (std::make_pair(false, locationContext));
				}
				current_token_ = parse_start;
				return (std::make_pair(false, locationContext));
			}
			if (setLocationDirectiveParameter(locationContext, directives.second) == false)
				return (std::make_pair(false, locationContext));
			token_pair = expectToken(OPERATOR, "}");
		}
		return (std::make_pair(true, locationContext));
	}

	std::pair<bool, std::vector<Directive> > Parser::parseContextBody(enum DirectiveKind kind)
	{
		std::vector<Directive>		directives;
		std::pair<bool, Directive> 	simple_directive_pair = expectSimpleDirective(kind);

		while (simple_directive_pair.first == true)
		{
			directives.push_back(simple_directive_pair.second);
			simple_directive_pair = expectSimpleDirective(kind);
		}
		if (simple_directive_pair.first == false) // check if it is a context or an error
		{
			if (kind == HTTP)
			{
				if (simple_directive_pair.second.directive == SERVER)
					return (std::make_pair(true, directives));
				if (simple_directive_pair.second.directive == HTTP ||
							simple_directive_pair.second.directive == LOCATION)
				{
					std::cout << "Error: There can't be any other block than server in http block, current directive is `";
					std::cout << sDirectiveKindStrings[simple_directive_pair.second.directive] << "` in line ";
					std::cout << current_token_->line_num << ".\n";
					return (std::make_pair(false, directives));
				}
			}
			else if (kind == SERVER)
			{
				if (simple_directive_pair.second.directive == LOCATION)
					return (std::make_pair(true, directives));
				if ((simple_directive_pair.second.directive == SERVER) ||
						(simple_directive_pair.second.directive == HTTP))
				{
					std::cout << "Error: There can't be any other block than location in server block, current directive is `";
					std::cout << sDirectiveKindStrings[simple_directive_pair.second.directive] << "` in line ";
					std::cout << current_token_->line_num << ".\n";
					return (std::make_pair(false, directives));
				}
			}
			if (kind == LOCATION && ((simple_directive_pair.second.directive == SERVER) ||
						(simple_directive_pair.second.directive == HTTP) ||
						(simple_directive_pair.second.directive == LOCATION)))
			{
				std::cout << "Error: There can't be any other block in location block.\n";
				return (std::make_pair(false, directives));
			}
			if (expectToken(OPERATOR, "}").first == true)
			{
				--current_token_;
				return (std::make_pair(true, directives));
			}
		}
		return (std::make_pair(false, directives));
	}

	std::pair<bool, Directive>	Parser::expectSimpleDirective(enum DirectiveKind kind)
	{
		(void)ft::sTokenTypeStrings[current_token_->type]; // for debuging
		std::vector<Token>::iterator 	start_token = current_token_;
		std::pair<bool, Directive> 	directive_pair = checkValidDirective();

		if (directive_pair.first == true)
		{
			if ((directive_pair.second.directive == HTTP) ||
				(directive_pair.second.directive == SERVER) ||
				(directive_pair.second.directive == LOCATION)) // check block directive
			{
				current_token_ = start_token;
				return (std::make_pair(false, directive_pair.second));
			}
			if (kind == HTTP && 
				(directive_pair.second.directive == LIMIT_EXCEPT || 
				directive_pair.second.directive == LISTEN || 
				directive_pair.second.directive == SERVER_NAME || 
				directive_pair.second.directive == RETURN || 
				directive_pair.second.directive == CGI || 
				directive_pair.second.directive == CGI_PATH))
			{
				std::cout << "Error: There can't be " << sDirectiveKindStrings[directive_pair.second.directive];
				std::cout << " directive in http block.\n";
				current_token_ = start_token;
				return (std::make_pair(false, directive_pair.second));
			}
			if (kind == SERVER && 
				(directive_pair.second.directive == LIMIT_EXCEPT || 
				directive_pair.second.directive == CGI || 
				directive_pair.second.directive == CGI_PATH))
			{
				std::cout << "Error: There can't be " << sDirectiveKindStrings[directive_pair.second.directive];
				std::cout << " directive in server block.\n";
				current_token_ = start_token;
				return (std::make_pair(false, directive_pair.second));
			}
			if (kind == LOCATION && 
				(directive_pair.second.directive == LISTEN || 
				directive_pair.second.directive == SERVER_NAME))
			{
				std::cout << "Error: There can't be " << sDirectiveKindStrings[directive_pair.second.directive];
				std::cout << " directive in location block.\n";
				current_token_ = start_token;
				return (std::make_pair(false, directive_pair.second));
			}
		
		}
		else if (directive_pair.first == false)
		{
			if (current_token_ != end_token_ && expectToken(OPERATOR, "}").first == false)
				std::cout << "Error: Unexpected directive, " << current_token_->text << " in line, " << current_token_->line_num << "\n";
			current_token_ = start_token;
			return (std::make_pair(false, directive_pair.second));
		}
		return (checkValidParameterNumber(directive_pair));
	}

	std::pair<bool, Directive> Parser::checkValidParameterNumber(std::pair<bool, Directive>& directive_pair)
	{
		std::vector<Token>::iterator 	start_token = current_token_;
		std::pair<bool, Token> 		parameter_token_pair = expectToken(PARAMETER);

		if (parameter_token_pair.first == true)
		{
			if ((directive_pair.second.directive == SERVER_NAME) ||
					(directive_pair.second.directive == LIMIT_EXCEPT) ||
					(directive_pair.second.directive == INDEX) ||
					(directive_pair.second.directive == RETURN))
			{
				while (parameter_token_pair.first == true)
				{
					directive_pair.second.parameters.push_back(parameter_token_pair.second.text);
					parameter_token_pair = expectToken(PARAMETER);
				}
			}
			else
				directive_pair.second.parameters.push_back(parameter_token_pair.second.text);
		}
		else
		{
			std::cout << "Error: Simple directive should at least one parameter\n";
			current_token_ = start_token;
			return (std::make_pair(false, directive_pair.second));
		}
		parameter_token_pair = expectToken(PARAMETER);
		if (parameter_token_pair.first == true)
		{
			current_token_ = start_token;
			std::cout << "Error: There can't be more parameters with directive " << directive_pair.second.name << ", ";
			std::cout << "additional parameter is " << parameter_token_pair.second.text<< ".\n";
			return (std::make_pair(false, directive_pair.second));
		}
		std::pair<bool, Token> token_pair = expectToken(OPERATOR, ";");
		if (token_pair.first == false)
		{
			current_token_ = start_token;
			std::cout << "Error: Simple directive should be terminated by ';' \n";
			return (std::make_pair(false, directive_pair.second));
		}
		return (std::make_pair(true, directive_pair.second));
	}
}
