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

namespace duckhacker
{
	namespace world
	{
		enum class BotAction
		{
			MOVE
		};

		class Bot
		{
		public:
			Bot(int id, int x, int y, int z);
			~Bot();

			int id;
			std::string code;

			const int& GetX();
			const int& GetY();
			const int& GetZ();
			const glm::vec3& GetDisplayCoords();

		private:
			int x_, y_, z_;

			int target_x_, target_y_, target_z_;
			float anim_counter_ = 0;
			bool anim_happening_ = false;
			glm::vec3 display_coords_;

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

		public:
			void HandleWarning_(const char * msg);
			void HandleError_();

		private:
			std::thread execute_thread_;

			std::atomic_bool action_available_;

			BotAction action_type_;
			int action_coords_[3];

			bool action_done_;
			std::mutex action_done_mutex_;
			std::condition_variable action_done_condition_;

			lua_State * lua_state_;
			std::jmp_buf preexec_state;
		};
	}
}

#endif