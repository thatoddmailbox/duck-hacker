#ifndef _WORLD_BOT_HPP
#define _WORLD_BOT_HPP

#include <csetjmp>

#include <string>

#include <lua.hpp>

namespace duckhacker
{
	namespace world
	{
		class Bot
		{
		public:
			Bot();
			~Bot();

			int id;
			int x, y, z;
			std::string code;

		public:
			void HandleWarning_(const char * msg);
			void HandleError_();

		private:
			lua_State * lua_state_;
			std::jmp_buf preexec_state;
		};
	}
}

#endif