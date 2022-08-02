#include <iostream>
#include "Tokenizer.hpp"

namespace ft
{
	std::vector<Token> Tokenizer::parse(const std::string& texts)
	{
		std::vector<Token>		tokens;
		Token				currentToken;
		std::string::const_iterator	it;
		
		currentToken.type = WHITESPACE;
		currentToken.lineNumber = 1;
		for (it = texts.begin(); it != texts.end(); ++it)
		{
			switch (*it)
			{
				case '{':
				case '}':
				case '=':
				case ';':
					if (currentToken.type != COMMENT && currentToken.type != PARAMETER)
					{
						endToken(currentToken, tokens);
						currentToken.type = OPERATOR;
						currentToken.text.append(1, *it);
						endToken(currentToken, tokens);
					}
					else
						currentToken.text.append(1, *it);
					break ;
				case ' ':
				case '\t':
					if (currentToken.type == PARAMETER || currentToken.type == COMMENT)
						currentToken.text.append(1, *it);
					else
						endToken(currentToken, tokens);
					break ;
				case '\r':
				case '\n':
					endToken(currentToken, tokens);
					++currentToken.lineNumber;
					break ;
				case '"':
					if (currentToken.type == COMMENT)
						currentToken.text.append(1, *it);
					else if (currentToken.type != PARAMETER)
					{
						endToken(currentToken, tokens);
						currentToken.type = PARAMETER;
					}
					else if (currentToken.type == PARAMETER)
						endToken(currentToken, tokens);
					break ;
				case '#':
					if (currentToken.type == WHITESPACE)
					{
						endToken(currentToken, tokens);
						currentToken.type = COMMENT;
						currentToken.text.erase();
					}
					else if (currentToken.type == COMMENT)
						currentToken.text.append(1, *it);
					break ;
				default:
					if (currentToken.type == WHITESPACE)
					{
						endToken(currentToken, tokens);
						currentToken.type = IDENTIFIER;
						currentToken.text.append(1, *it);
					}
					else 
						currentToken.text.append(1, *it);
					break ;
			}
		}
		endToken(currentToken, tokens);
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
		std::cout << "Token(" << sTokenTypeStrings[type] << ", \"" << text << "\", " << lineNumber << ")" << std::endl;
	}
/*
	Token::Token()
	{
		this->type = WHITESPACE;	
		this->lineNumber = 1;
	}
*/
}
