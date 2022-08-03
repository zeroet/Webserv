#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include <vector>
#include <string>
#include "BaseDirectives.hpp"
#include "HttpBlock.hpp"
#include "LocationBlock.hpp"
#include <sstream>

namespace ft
{
	class HttpBlock;
	class LocationBlock;

	class ServerBlock : public BaseDirectives
	{
		private:
			unsigned int			listen_;
			std::vector<std::string>	serverName_;
			std::vector<std::string>	return_;
			int 				socketFd_;

		public:
			std::vector<LocationBlock>	locationList;
		public:
			ServerBlock(const BaseDirectives& context);
			// Getter
			//const std::vector<LocationBlock>	getLocationBlock();
			unsigned int			getListen(void) const;
			const std::vector<std::string>	getServerName(void) const;
			const std::vector<std::string>	getReturn(void) const;
			int				getSocketFd(void) const;


			// Setter
			void				setListen(const unsigned int x);
			void				setServerName(const std::string x);
			void				setReturn(const std::string x);
			void				setSocketFd(const int i);

			void				clearServerName(void);
	};

}

#endif
