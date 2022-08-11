#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include <iostream>
#include <string>
#include <vector>

namespace ft
{
	enum TokenType
	{
		WHITESPACE,
		IDENTIFIER,
		DIRECTIVE,
		PARAMETER,
		OPERATOR,
		COMMENT
	};

	static const char* sTokenTypeStrings[] = {
		"WHITESPACE",
		"IDENTIFIER",
		"DIRECTIVE",
		"PARAMETER",
		"OPERATOR",
		"COMMENT"
	};

	class Token
	{
		public:
			enum TokenType 	type;
			std::string	text;
			size_t		line_num;

		public:
			//Token();
			void	debugPrint() const;
	};

	class Tokenizer
	{
		private:
			std::vector<Token>::iterator	current_token_;
			std::vector<Token>::iterator	end_token_;
		public:
			std::vector<Token>	 	parse(const std::string& texts);
		private:
			void 				endToken(Token& token, std::vector<Token>& tokens);
	};
}
#endif
