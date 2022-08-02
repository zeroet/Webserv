#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include <vector>
#include <string>
#include "BaseDirectives.hpp"
#include "HttpBlock.hpp"
#include "LocationBlock.hpp"

namespace ft
{
	class HttpBlock;
	class LocationBlock;

	class ServerBlock : public BaseDirectives
	{
		private:
			std::string			listen_;
			std::vector<std::string>	serverName_;
			std::vector<std::string>	return_;
		public:
			std::vector<LocationBlock>	locationList;
		public:
			ServerBlock(const BaseDirectives& context);
			// Getter
			//const std::vector<LocationBlock>	getLocationBlock();
			const std::string		getListen(void) const;
			const std::vector<std::string>	getServerName(void) const;
			const std::vector<std::string>	getReturn(void) const;

			// Setter
			void				setListen(const std::string x);
			void				setServerName(const std::string x);
			void				setReturn(const std::string x);

			void				clearServerName(void);
	};

}

#endif
