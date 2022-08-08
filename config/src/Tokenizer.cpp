#include <iostream>
#include "Tokenizer.hpp"

namespace ft
{
	std::vector<Token> Tokenizer::parse(const std::string& texts)
	{
		std::vector<Token>		tokens;
		Token				current_token;
		std::string::const_iterator	it;
		
		current_token.type = WHITESPACE;
		current_token.line_num = 1;
		for (it = texts.begin(); it != texts.end(); ++it)
		{
			switch (*it)
			{
				case '{':
				case '}':
				case '=':
				case ';':
					if (current_token.type != COMMENT && current_token.type != PARAMETER)
					{
						endToken(current_token, tokens);
						current_token.type = OPERATOR;
						current_token.text.append(1, *it);
						endToken(current_token, tokens);
					}
					else
						current_token.text.append(1, *it);
					break ;
				case ' ':
				case '\t':
					if (current_token.type == PARAMETER || current_token.type == COMMENT)
						current_token.text.append(1, *it);
					else
						endToken(current_token, tokens);
					break ;
				case '\r':
				case '\n':
					endToken(current_token, tokens);
					++current_token.line_num;
					break ;
				case '"':
					if (current_token.type == COMMENT)
						current_token.text.append(1, *it);
					else if (current_token.type != PARAMETER)
					{
						endToken(current_token, tokens);
						current_token.type = PARAMETER;
					}
					else if (current_token.type == PARAMETER)
						endToken(current_token, tokens);
					break ;
				case '#':
					if (current_token.type == WHITESPACE)
					{
						endToken(current_token, tokens);
						current_token.type = COMMENT;
						current_token.text.erase();
					}
					else if (current_token.type == COMMENT)
						current_token.text.append(1, *it);
					break ;
				default:
					if (current_token.type == WHITESPACE)
					{
						endToken(current_token, tokens);
						current_token.type = IDENTIFIER;
						current_token.text.append(1, *it);
					}
					else 
						current_token.text.append(1, *it);
					break ;
			}
		}
		endToken(current_token, tokens);
		return (tokens);
	}

	void	Tokenizer::endToken(Token& token, std::vector<Token>& tokens)
	{
		if (token.type == COMMENT)
			std::cout << "ignoring comment: " + token.text << std::endl;
		else if (token.type != WHITESPACE)
			tokens.push_back(token);
		token.type = WHITESPACE;
		token.text.erase();
	}

	void	Token::debugPrint() const
	{
		std::cout << "Token(" << sTokenTypeStrings[type] << ", \"" << text << "\", " << line_num << ")" << std::endl;
	}
/*
	Token::Token()
	{
		this->type = WHITESPACE;	
		this->line_num = 1;
	}
*/
}
