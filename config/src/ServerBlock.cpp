#include "ServerBlock.hpp"

namespace ft
{
	ServerBlock::ServerBlock(const BaseDirectives& context)
	{
		this->root_ = context.getRoot();
		this->client_max_body_size_ = context.getClientMaxBodySize();
		this->keepalive_timeout_ = context.getKeepaliveTimeout();
		this->index_ = context.getIndex();
		this->autoindex_ = context.getAutoindex();
		this->error_page_ = context.getErrorPage();
		//setErrorPage(context.getErrorPage());
		this->listen_ = 80;
		this->server_name_.push_back("");
		this->socket_fd_ = -1;
	}

	// Getter
	const std::vector<LocationBlock>	ServerBlock::getLocationBlock() const
	{
		return (this->location_list);
	}

	const std::pair<bool, LocationBlock>	ServerBlock::getLocationBlock(const std::string path) const
	{
		LocationBlock					return_location;
		std::vector<LocationBlock>::const_iterator	current_location = location_list.begin();
		std::vector<LocationBlock>::const_iterator	end_location = location_list.end();
		std::vector<std::pair<size_t, size_t> >		vector_pair;


		for (; current_location != end_location; ++current_location)
		{
			//if (current_location->getUriPath().compare(path, ) == 0)
			if (path.compare(0, current_location->getUriPath().length(), current_location->getUriPath()) == 0)
				return (std::make_pair(true, *current_location));
			else
			{
				std::string current_path = current_location->getUriPath();
				std::cout << "path: " << path << ", " << current_path << "\n";
				size_t	index = 0;
				char	current_char;
				while (current_path[index])
				{
					current_char = current_path[index];
					if (path[index] != current_char)
					{
						index--;
						vector_pair.push_back(std::make_pair(index, current_path.length()));
						break ;
					}
					index++;
				}

				std::vector<std::pair<size_t, size_t> >::const_iterator		current_vector_pair;
				std::vector<std::pair<size_t, size_t> >::const_iterator		end_vector_pair;
				current_vector_pair = vector_pair.begin();
				end_vector_pair = vector_pair.end();
				std::pair<size_t, size_t> length_pair = std::make_pair(0, 0);

				size_t	longest_matched_index = 0;
				index = 0;

				for (; current_vector_pair != end_vector_pair; ++current_vector_pair)
				{
					std::cout << "(match, rui): " << current_vector_pair->first << ", " << current_vector_pair->second << "\n";
					if (current_vector_pair->first > length_pair.first)
					{
						length_pair.first = current_vector_pair->first;
						length_pair.second = current_vector_pair->second;
						longest_matched_index = index;
					}
					else if (current_vector_pair->first == length_pair.first)
					{
						size_t second_index = 0;
						while (second_index < vector_pair.size())
						{
							if (current_vector_pair->second < length_pair.second)
							{
								length_pair.second = current_vector_pair->second;
								longest_matched_index = index;
							}
							// error handling
							// case 0. if there are two location blocks which have the same uri path?
							// case 1. reqest rui path: /cgi_teste, location ruis: /cgi_testlr/, /cgi_tesi/, /cgitest/,
							// case 2. if it doesn't end with / at the end?
							second_index++;
						}
					}
					index++;
				}
				std::cout << "index: " << index << " longest matched index: " << longest_matched_index << "\n";
				//if (longest_matched_index < index)
				//	return (std::make_pair(true, location_list.at(index)));
			}
		}
		return (std::make_pair(false, return_location));
	}

	unsigned int		ServerBlock::getListen(void) const
	{
		return (this->listen_);
	}

	const std::vector<std::string>	ServerBlock::getServerName(void) const
	{
		return (this->server_name_);
	}

	const std::string		ServerBlock::getServerName(size_t index) const
	{
		return (this->server_name_.at(index));
	}

	const std::vector<std::string>	ServerBlock::getReturn(void) const
	{
		return (this->return_);
	}

	int	ServerBlock::getSocketFd(void) const
	{
		return (this->socket_fd_);
	}

	// Setter
	void				ServerBlock::setListen(const unsigned int x)
	{
		this->listen_ = x;
	}

	void				ServerBlock::setServerName(const std::string x)
	{
		this->server_name_.push_back(x);
	}

	void				ServerBlock::setReturn(const std::string x)
	{
		this->return_.push_back(x);
	}

	void				ServerBlock::setSocketFd(const int i)
	{
		this->socket_fd_ = i;
	}

	void				ServerBlock::clearServerName(void)
	{
		this->server_name_.clear();
	}
}
