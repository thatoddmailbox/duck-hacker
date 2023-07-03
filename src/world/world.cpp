#include "world/world.hpp"

namespace duckhacker
{
	namespace world
	{
		World::World()
		{
			// TODO: should load this from some file or something
			Bot * bot = new Bot();
			bot->id = 1;
			bot->x = 5;
			bot->y = 0;
			// bot.z = 4;
			bots.push_back(bot);
		}

		World::~World()
		{
			for (Bot * bot : bots)
			{
				delete bot;
			}
		}
	}
}