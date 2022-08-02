#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <cstdlib>
#include "Tokenizer.hpp"
#include "Directive.hpp"
#include "HttpBlock.hpp"
#include "ServerBlock.hpp"
#include "LocationBlock.hpp"

namespace ft
{
	class Parser
	{
		private:
			std::vector<Token>::iterator 			currentToken_;
			std::vector<Token>::iterator 			startToken_;
			std::vector<Token>::iterator			endToken_;
			std::map<std::string, Directive>		directives_;

		public:
			Parser();
			std::pair<bool, HttpBlock>			parse(std::vector<Token>& tokens);
		private:
			void						modifyIdentifierToken(std::vector<Token>& tokens);

			std::pair<bool, Directive>			checkValidDirective();
			std::pair<bool, Directive> 			checkValidParameterNumber(std::pair<bool, Directive>& validDirective);

 			// Empty string means match any string.
			std::pair<bool, Token>				expectToken(enum TokenType type, const std::string& name = std::string());
			std::pair<bool, Directive>			expectHttpContext();
			std::pair<bool, Directive>			expectServerContext();
			std::pair<bool, Directive>			expectLocationContext();
			std::pair<bool, Directive> 			expectSimpleDirective(enum DirectiveKind kind);

			std::pair<bool, std::vector<Directive> >	parseContextBody(enum DirectiveKind kind);
			std::pair<bool, HttpBlock>			parseHttpContext(std::pair<bool, Directive> validDirective);
			std::pair<bool, ServerBlock>			parseServerContext(HttpBlock& httpContext, std::pair<bool, Directive> validDirective);
			std::pair<bool, LocationBlock>			parseLocationContext(ServerBlock& serverContext);

			bool						setBaseDirectiveParameter(BaseDirectives& context, std::vector<Directive>::iterator& currentDirective);
			bool						setHttpDirectiveParameter(HttpBlock& context, std::vector<Directive> directiveList);
			bool						setServerDirectiveParameter(ServerBlock& context, std::vector<Directive> directiveList);
			bool						setLocationDirectiveParameter(LocationBlock& context, std::vector<Directive> directiveList);

	};
}
#endif
