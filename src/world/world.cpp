#include "world/world.hpp"

namespace duckhacker
{
	namespace world
	{
		World::World()
		{
			// TODO: should load this from some file or something
			Bot bot;
			bot.z = 1;
			bots.push_back(bot);
		}
	}
}