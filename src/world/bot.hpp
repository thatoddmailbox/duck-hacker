#ifndef _WORLD_BOT_HPP
#define _WORLD_BOT_HPP

#include <atomic>
#include <condition_variable>
#include <csetjmp>
#include <mutex>
#include <string>
#include <thread>

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
			Bot();
			~Bot();

			int id;
			int x, y, z;
			std::string code;

			void Execute();

			void Update(float dt);

		public:
			static void EnterExecuteThread_(Bot * b);
			void RequestStop();
			void WaitForStop();

		private:
			void Execute_();

		public:
			void HandleWarning_(const char * msg);
			void HandleError_();

		private:
			std::thread execute_thread_;

			std::atomic_bool action_available;

			BotAction action_type;
			int action_coords[3];

			bool action_done;
			std::mutex action_done_mutex;
			std::condition_variable action_done_condition;

			lua_State * lua_state_;
			std::jmp_buf preexec_state;
		};
	}
}

#endif