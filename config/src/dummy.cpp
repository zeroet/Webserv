
	std::pair<bool, HttpBlock>	Parser::expectHttpContext()
	{
		std::vector<Token>::iterator parseStart = currentToken_;
		std::pair<bool, Directive> possibleValidDirective = checkValidDirective();
		std::pair<bool, std::vector<Directive> > directives;
		HttpBlock	httpContext;

		if (possibleValidDirective.first == true) // When we have a block directive name
		{
			std::pair<bool, Token> possibleOperator = expectOperator("{");
			if (possibleOperator.first == true) // When we have a block directive
			{
				//std::cout << "We have a block directive: " << possibleValidDirective.second.name << "\n";
				if (possibleValidDirective.second.directive != HTTP)
				{
					std::cout << "Error: It should start with http context but the directive is " << possibleValidDirective.second.name << "\n";
					currentToken_ = parseStart;
					return (std::make_pair(false, httpContext));
				}
				return(parseHttpContext(possibleValidDirective));
/*
				while (expectOperator("}").first == false)
				{
					possibleValidDirective = expectServerContext();
					if (possibleValidDirective.first == false)
					{
						std::cout << "there is no server block, start to parse context body, ";
						std::cout << "currentToken is :" << currentToken_->text << "\n";
						directives = parseContextBody();
						if (!directives.first)
						{
							currentToken_ = parseStart;
							return (std::make_pair(false, httpContext));
						}
						httpContext.directiveList.insert(httpContext.directiveList.end(), directives.second.begin(), directives.second.end());
						//directives.second.insert(directives.second.end(), tempDirectives.second.begin(), tempDirectives.second.end());
					}
					else
					{
						std::pair<bool, ServerBlock>	possibleServerContext = parseServerContext(possibleValidDirective);
						
						//std::cout << "there is no server block, start to parse context body, ";
						//std::cout << "currentToken is :" << currentToken_->text << "\n";
						//directives = parseContextBody();
						if (possibleServerContext.first == false)
						//if (!directives.first)
						{
							currentToken_ = parseStart;
							return (std::make_pair(false, httpContext));
						}
						//serverContext.directiveList.insert(serverContext.directiveList.end(), directives.second.begin(), directives.second.end());
						httpContext.serverList.push_back(possibleServerContext.second);
						std::cout << "One server context has been successfuly enclosed with a closing curly braket" << "\n";
						std::vector<Directive>::iterator	currtToken = possibleServerContext.second.directiveList.begin();
						std::vector<Directive>::iterator	endToken = possibleServerContext.second.directiveList.end();
						for (; currtToken != endToken; ++currtToken)
							std::cout << "serverContext Directive: " << (*currtToken).name << "\n";
					}

				}
				std::cout << "Http context has been successfuly enclosed with a closing curly braket" << "\n";
				//std::cout << "in checkValidParameterNumber :" << validDirective.second.name << "\n";
				//std::vector<Directive>::iterator	currtToken = directives.second.begin();
				//std::vector<Directive>::iterator	endToken = directives.second.end();
				std::vector<Directive>::iterator	currtToken = httpContext.directiveList.begin();
				std::vector<Directive>::iterator	endToken = httpContext.directiveList.end();
				for (; currtToken != endToken; ++currtToken)
					std::cout << "httpContext Directive: " << (*currtToken).name << "\n";
				//if (returnHttpContext.first == false)
				//return (returnHttpContext);
*/
				return (std::make_pair(true, httpContext));
			}
			else
				currentToken_ = parseStart;
				//--currentToken_;
		}
		return (std::make_pair(false, httpContext));
	}


	bool	Parser::expectContext()
	{
		std::vector<Token>::iterator parseStart = currentToken_;
		std::pair<bool, Directive> possibleValidDirective = checkValidDirective();
		//std::pair<bool, std::string> possibleDirective = checkValidDirective();

		if (possibleValidDirective.first == true) // When we have a block directive name
		{
			std::pair<bool, Token> possibleOperator = expectOperator("{");
			if (possibleOperator.first == true) // When we have a block directive
			{
				//HttpBlock	http;
				std::cout << "We have a block directive: " << possibleValidDirective.second.name << "\n";
				//if (sDirectiveKindStrings[possibleDirective.second.directive] != "http")
/*
				if (possibleDirective.second.directive != HTTP)
				{
					std::cout << "It should start with http context but the directive is " << possibleDirective.second.name << "\n";
					currentToken_ = parseStart;
					return (false);
				}
*/
				//if ()
				//	parseUriPath();
				//parseContextBody();
				//std::pair<bool, std::vector<Statement> > statements = parseContextBody();
				if (possibleValidDirective.second.directive == HTTP)
				{
					std::pair<bool, std::vector<Directive> > directives = parseContextBody();
					if (!directives.first)
					{
						currentToken_ = parseStart;
						return (false);
					}
				}
				else if (possibleValidDirective.second.directive == SERVER)
				{
					std::pair<bool, std::vector<Directive> > directives = parseContextBody();
					if (!directives.first)
					{
						currentToken_ = parseStart;
						return (false);
					}
				}
				return (true);
			}
			else
			{
				//possibleValidDirective = checkValidDirective();
				std::cout << "possibleValidDirective: " << possibleValidDirective.second.name << "\n";
				if (possibleValidDirective.first == true) // When we have a location block
				{
					std::cout << "We have a location block: " << possibleValidDirective.second.name << "\n";
					if (possibleValidDirective.second.directive == LOCATION)
					{
						std::pair<bool, Token> possibleParameter = expectParameter();
						if (possibleParameter.first == true)
							possibleValidDirective.second.parameters.push_back(possibleParameter.second.text);
						else
						{
							std::cout << "Error no parameter: Location directive should have one URI path\n";
							currentToken_ = parseStart;
							return (false);
						}
						std::pair<bool, Token> possibleOperator = expectOperator("{");
						if (possibleOperator.first == false)
						{
							std::cout << "Error mutiple parameters: Location directive should have one URI path\n";
							currentToken_ = parseStart;
							return (false);
						}
						else
						{
							std::vector<std::string>::iterator	currtToken = possibleValidDirective.second.parameters.begin();
							std::vector<std::string>::iterator	endToken = possibleValidDirective.second.parameters.end();
							for (; currtToken != endToken; ++currtToken)
								std::cout << "parameter: " << *currtToken << "\n";
							std::pair<bool, std::vector<Directive> > directives = parseContextBody();
							if (!directives.first)
							{
								currentToken_ = parseStart;
								return (false);
							}
						}

					}
				}
				
				std::cout << "no block directive: " << possibleValidDirective.second.name << "\n";
				currentToken_ = parseStart;
				//--currentToken_;
			}
		}
		return (false);
	}
