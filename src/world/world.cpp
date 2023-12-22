#include "world/world.hpp"

namespace duckhacker
{
	namespace world
	{
		World::World()
		{
			// TODO: should load this from some file or something
			Bot * bot = new Bot(1, -2, 1, 4, 90);
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

		void World::Update(float dt)
		{
			if (state_ != WorldState::RUNNING)
			{
				return;
			}

			for (Bot * bot : bots)
			{
				bot->Update(dt);
			}
		}
	}
}