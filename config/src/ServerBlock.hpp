#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include "BaseDirectives.hpp"
#include "HttpBlock.hpp"
#include "LocationBlock.hpp"
#include <iostream>

namespace ft
{
	class HttpBlock;
	class LocationBlock;

	class ServerBlock : public BaseDirectives
	{
		private:
			unsigned int			listen_;
			std::vector<std::string>	server_name_;
			std::vector<std::string>	return_;
			int 				socket_fd_;

		public:
			std::vector<LocationBlock>	location_list;
		public:
			ServerBlock();
			ServerBlock(int i) { (void)i;
			std::cout << "server block not found" << std::endl; };
			ServerBlock(const BaseDirectives& context);
			ServerBlock(const ServerBlock& other);
			ServerBlock& operator= (const ServerBlock& other);
			~ServerBlock();
			// Getter
			const std::vector<LocationBlock>	getLocationBlock() const;
			const std::pair<bool, LocationBlock>	getLocationBlock(const std::string& request_path) const;
			unsigned int				getListen(void) const;
			const std::vector<std::string>		getServerName(void) const;
			const std::string			getServerName(size_t index) const;
			const std::vector<std::string>		getReturn(void) const;
			int					getSocketFd(void) const;

			// Setter
			void					setListen(const unsigned int x);
			void					setServerName(const std::vector<std::string>& x);
			void					setServerName(const std::string& x);
			void					setReturn(const std::vector<std::string>& x);
			void					setReturn(const std::string& x);
			void					setSocketFd(const int i);

			void					clearServerName(void);
			bool					checkLocationUriPath(const std::string& uri_path) const;
			bool					checkServerName(const std::string& request_server_name) const;
	};

}

#endif
