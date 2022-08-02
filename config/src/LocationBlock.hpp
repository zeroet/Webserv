#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

#include <vector>
#include <string>
#include "BaseDirectives.hpp"

namespace ft
{
	class LocationBlock : public BaseDirectives
	{
		private:
			std::vector<std::string>	limitExcept_;
			std::vector<std::string>	return_;
		public:
			LocationBlock(const BaseDirectives& context);
			// Getter
			const std::vector<std::string>	getLimitExcept(void) const;
			const std::vector<std::string>	getReturn(void) const;

			// Setter
			void				setLimitExcept(const std::string x);
			void				setReturn(const std::string x);
	};

}

#endif
