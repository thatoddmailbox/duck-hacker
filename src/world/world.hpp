#ifndef _WORLD_WORLD_HPP
#define _WORLD_WORLD_HPP

#include <atomic>
#include <vector>

#include "glm/glm.hpp"

#include "content/manager.hpp"

#include "render/object.hpp"

#include "world/bot.hpp"
#include "world/console.hpp"
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

			world::Console& GetConsole();

			const std::string& GetMission();
			const std::string& GetMissionGoal();

			const State& GetState();

			const std::atomic_int& GetTicks();

			void Run();
			void Stop();

			void Update(float dt);

		private:
			world::Console console_;

			std::atomic_int ticks_ = ATOMIC_VAR_INIT(0);
			float ticks_accum_ = 0;

			std::vector<glm::vec4> bot_reset_positions_;
			std::vector<glm::vec4> npc_reset_positions_;

			std::string mission_;
			std::string mission_goal_;

			glm::vec3 center_point_;
			State state_ = State::READY;
		};
	}
}

#endif