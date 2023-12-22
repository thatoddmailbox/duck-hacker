#ifndef _WORLD_WORLD_HPP
#define _WORLD_WORLD_HPP

#include <vector>

#include "content/manager.hpp"

#include "render/object.hpp"

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
			World(content::Manager * content_manager, std::string file_path);
			~World();

			std::vector<Bot *> bots;
			std::vector<render::Object> objects;

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