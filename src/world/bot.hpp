#ifndef _WORLD_BOT_HPP
#define _WORLD_BOT_HPP

#include <atomic>
#include <condition_variable>
#include <csetjmp>
#include <mutex>
#include <string>
#include <thread>

#include <glm/glm.hpp>

#include <lua.hpp>

#include "content/manager.hpp"

#include "render/object.hpp"

namespace duckhacker
{
	namespace world
	{
		enum class BotAction
		{
			MOVE,
			TURN
		};

		class Bot
		{
		public:
			Bot(content::Manager * content_manager, int id, int x, int y, int z, int rotation);
			~Bot();

			std::string code;

			render::Object object;

			const int& GetID();
			const int& GetX();
			const int& GetY();
			const int& GetZ();
			const int& GetRotation();

			const glm::vec3& GetDisplayCoords();
			const float& GetDisplayRotation();

		private:
			int id_;
			int x_, y_, z_;
			int rotation_;

			int target_x_, target_y_, target_z_;
			int target_rotation_;
			int target_rotation_display_;
			float anim_counter_ = 0;
			bool anim_happening_ = false;
			glm::vec3 display_coords_;
			float display_rotation_;

		public:
			void Execute();

			void Update(float dt);

		public:
			static void EnterExecuteThread_(Bot * b);
			void RequestStop();
			void WaitForStop();

		private:
			void Execute_();

		public:
			int OnLuaCall_Move_(int dx, int dy, int dz);
			int OnLuaCall_Turn_(int da);

		public:
			void HandleWarning_(const char * msg);
			void HandleError_();

		private:
			std::thread execute_thread_;

			std::atomic_bool action_available_;

			BotAction action_type_;
			int action_coords_[3];
			int action_angle_;

			bool action_done_;
			std::mutex action_done_mutex_;
			std::condition_variable action_done_condition_;

			lua_State * lua_state_;
			std::jmp_buf preexec_state;
		};
	}
}

#endif