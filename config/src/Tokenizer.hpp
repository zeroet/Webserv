#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
//#include "Parser.hpp" 
//#include "Directive.hpp"


namespace ft
{
	enum TokenType
	{
		WHITESPACE,
		IDENTIFIER,
		DIRECTIVE,
		PARAMETER,
		OPERATOR,
		//STRING_LITERAL,
		COMMENT
	};

	//static const char* sTokenTypeStrings[] = {
	static const char* sTokenTypeStrings[] = {
		"WHITESPACE",
		"IDENTIFIER",
		"DIRECTIVE",
		"PARAMETER",
		"OPERATOR",
		//"STRING_LITERAL",
		"COMMENT"
	};

	class Token
	{
		public:
			enum TokenType 	type;
			std::string	text;
			size_t		lineNumber;

		public:
			//Token();
			void	debugPrint() const;
		//private:
	};

	class Tokenizer
	{
		private:
			std::vector<Token>::iterator	currentToken_;
			std::vector<Token>::iterator	endToken_;
		public:
			std::vector<Token>	 	parse(const std::string& texts);
		private:
			void 				endToken(Token& token, std::vector<Token>& tokens);
			//void				checkIdentifierToken(std::vector<Token>& tokens);
	};
}
#endif
