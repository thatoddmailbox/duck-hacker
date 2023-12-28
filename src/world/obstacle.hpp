#ifndef _WORLD_OBSTACLE_HPP
#define _WORLD_OBSTACLE_HPP

#include "world/bot.hpp"

namespace duckhacker
{
	namespace world
	{
		enum class ObstacleType
		{
			WALL
		};

		struct Obstacle
		{
			ObstacleType type;
			int x;
			int y;
			int z;
			BotType blocks;
		};
	}
}

#endif
