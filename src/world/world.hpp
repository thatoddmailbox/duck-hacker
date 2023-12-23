#ifndef _WORLD_WORLD_HPP
#define _WORLD_WORLD_HPP

#include <vector>

#include "content/manager.hpp"

#include "render/object.hpp"

#include "world/bot.hpp"
#include "world/npc.hpp"

namespace duckhacker
{
	namespace world
	{
		enum class State
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
			std::vector<NPC *> npcs;
			std::vector<render::Object> objects;

			const glm::vec3& GetCenterPoint();

			const State& GetState();

			void Run();
			void Stop();

			void Update(float dt);

		private:
			glm::vec3 center_point_;
			State state_ = State::READY;
		};
	}
}

#endif