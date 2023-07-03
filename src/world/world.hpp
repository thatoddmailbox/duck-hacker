#ifndef _WORLD_WORLD_HPP
#define _WORLD_WORLD_HPP

#include <vector>

#include "world/bot.hpp"

namespace duckhacker
{
	namespace world
	{
		enum class WorldState
		{
			READY,
			RUNNING
		};

		class World
		{
		public:
			World();
			~World();

			std::vector<Bot *> bots;

			const WorldState& GetState();

			void Run();
			void Stop();

			void Update(float dt);

		private:
			WorldState state_ = WorldState::READY;
		};
	}
}

#endif