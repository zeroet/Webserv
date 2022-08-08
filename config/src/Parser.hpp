#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <cstdlib>
#include "Tokenizer.hpp"
#include "Directive.hpp"
#include "HttpBlock.hpp"

namespace ft
{
	class Parser
	{
		private:
			std::vector<Token>::iterator 			current_token_;
			std::vector<Token>::iterator 			start_token_;
			std::vector<Token>::iterator			end_token_;
			std::map<std::string, Directive>		directives_;

		public:
			Parser();
			std::pair<bool, HttpBlock>			parse(std::vector<Token>& tokens);
		private:
			void						modifyIdentifierToken(std::vector<Token>& tokens);

			std::pair<bool, Directive>			checkValidDirective();
			std::pair<bool, Directive> 			checkValidParameterNumber(std::pair<bool, Directive>& valid_directive);

 			// Empty string means match any string.
			std::pair<bool, Token>				expectToken(enum TokenType type, const std::string& name = std::string());
			std::pair<bool, Directive>			expectHttpContext();
			std::pair<bool, Directive>			expectServerContext();
			std::pair<bool, Directive>			expectLocationContext();
			std::pair<bool, Directive> 			expectSimpleDirective(enum DirectiveKind kind);

			std::pair<bool, std::vector<Directive> >	parseContextBody(enum DirectiveKind kind);
			std::pair<bool, HttpBlock>			parseHttpContext(std::pair<bool, Directive> valid_directive);
			std::pair<bool, ServerBlock>			parseServerContext(HttpBlock& httpContext, std::pair<bool, Directive> valid_directive);
			std::pair<bool, LocationBlock>			parseLocationContext(ServerBlock& server_context);

			bool						setBaseDirectiveParameter(BaseDirectives& context, std::vector<Directive>::iterator& currentDirective);
			bool						setHttpDirectiveParameter(HttpBlock& context, std::vector<Directive> directive_list);
			bool						setServerDirectiveParameter(ServerBlock& context, std::vector<Directive> directive_list);
			bool						setLocationDirectiveParameter(LocationBlock& context, std::vector<Directive> directive_list);

	};
}
#endif
