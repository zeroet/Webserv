#include "HttpBlock.hpp"

namespace ft
{
	HttpBlock::HttpBlock()
	{
	}

	HttpBlock::HttpBlock(const HttpBlock& other) : server_list(other.getServerBlock())
	{
		this->setRoot(other.getRoot());
		this->setClientMaxBodySize(other.getClientMaxBodySize());
		this->setKeepaliveTimeout(other.getKeepaliveTimeout());
		this->setIndex(other.getIndex());
		this->setAutoindex(other.getAutoindex());
		this->setErrorPage(other.getErrorPage());
	}

	HttpBlock::~HttpBlock()
	{
	}

	HttpBlock& HttpBlock::operator= (const HttpBlock& other)
	{
		if (this != &other)
		{
			this->setRoot(other.getRoot());
			this->setClientMaxBodySize(other.getClientMaxBodySize());
			this->setKeepaliveTimeout(other.getKeepaliveTimeout());
			this->setIndex(other.getIndex());
			this->setAutoindex(other.getAutoindex());
			this->setErrorPage(other.getErrorPage());
			this->server_list = other.getServerBlock();
		}
		return (*this);
	}

	std::vector<ServerBlock>	HttpBlock::getServerBlock() const
	{
		return (this->server_list);
	}

	// std::pair<bool, ServerBlock>	HttpBlock::getServerBlock(const unsigned int listen, const std::string& request_server_name) const
	// {
	// 	std::vector<ServerBlock>::const_iterator	current_server = this->server_list.begin();
	// 	std::vector<ServerBlock>::const_iterator	end_server = this->server_list.end();
	// 	std::vector<ServerBlock>			server_vector;
	// 	ServerBlock					return_server = server_list.at(0);

	// 	for (; current_server != end_server; ++current_server)
	// 	{
	// 		if (current_server->getListen() == listen)
	// 			server_vector.push_back(*current_server);
	// 	}
	// 	if (request_server_name == "")
	// 		return (std::make_pair(true, server_vector.at(0)));
	// 	else if (server_vector.size() == 1)
	// 		return (std::make_pair(true, server_vector.at(0)));
	// 	else if (server_vector.size() == 0)
	// 		return (std::make_pair(false, return_server));
	// 	else
	// 	{
	// 		std::vector<ServerBlock>::iterator	current_server = server_vector.begin();
	// 		std::vector<ServerBlock>::iterator	    end_server = server_vector.end();

	// 		//std::cout << "there are more than one server with listen " << listen << "\n";
	// 		for (; current_server != end_server; ++current_server)
	// 		{
	// 			if (current_server->checkServerName(listen, request_server_name) == true)
	// 			{
	// 				//std::cout << "matched server name is " << request_server_name << "\n";
	// 				return (std::make_pair(true, *current_server));
	// 			}
	// 		}
	// 	}
	// 	return (std::make_pair(false, return_server));
	// }
};
