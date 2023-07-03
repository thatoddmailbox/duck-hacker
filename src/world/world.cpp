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

		const WorldState& World::GetState()
		{
			return state_;
		}

		void World::Run()
		{
			state_ = WorldState::RUNNING;

			for (Bot * bot : bots)
			{
				bot->Execute();
			}
		}

		void World::Stop()
		{
			for (Bot * bot : bots)
			{
				bot->RequestStop();
			}

			for (Bot * bot : bots)
			{
				bot->WaitForStop();
			}

			// TOOD: reset the world or something
			state_ = WorldState::READY;
		}
	}
}