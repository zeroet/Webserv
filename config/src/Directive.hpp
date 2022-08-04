#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#define LAST_DIRECTIVE_KIND LIMIT_EXCEPT

namespace ft
{
	enum DirectiveKind
	{
		HTTP = 0,
		SERVER,
		LOCATION,
		CLIENT_MAX_BODY_SIZE,
		KEEPALIVE_TIMEOUT,
		INDEX,
		AUTOINDEX,
		ROOT,
		ERROR_PAGE,
		LISTEN,
		SERVER_NAME,
		RETURN,
		CGI,
		CGI_PATH,
		LIMIT_EXCEPT
	};

	static std::string  sDirectiveKindStrings[] = {
		"http",
		"server",
		"location",
		"client_max_body_size",
		"keepalive_timeout",
		"index",
		"autoindex",
		"root",
		"error_page",
		"listen",
		"server_name",
		"return",
		"cgi",
		"cgi_path",
		"limit_except"
	};

	class Directive
	{
		public:
			enum DirectiveKind		directive;
			std::string			name;
			std::vector<std::string>	parameters;

		public:
			Directive(enum DirectiveKind directive = HTTP, const std::string& name = std::string()) : directive(directive), name(name) {}
	};
}
#endif
