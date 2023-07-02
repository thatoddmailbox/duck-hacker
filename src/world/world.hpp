#ifndef _WORLD_WORLD_HPP
#define _WORLD_WORLD_HPP

#include <vector>

#include "world/bot.hpp"

namespace duckhacker
{
	namespace world
	{
		class World
		{
		public:
			World();
			~World();

			std::vector<Bot *> bots;
		};
	}
}

#endif