#include "world/bot.hpp"

namespace duckhacker
{
	namespace world
	{
		static void Bot_HandleWarning(void * ud, const char * msg, int tocont)
		{
			lua_State * L = (lua_State *) ud;
			(*((Bot **) lua_getextraspace(L)))->HandleWarning_(msg);
		}

		static int Bot_HandleError(lua_State * L)
		{
			(*((Bot **) lua_getextraspace(L)))->HandleError_();

			// technically, we should never return from the above function due to the longjmp
			return 0;
		}

		Bot::Bot()
		{
			id = 0;
			x = y = z = 0;
			code = "hi";

			lua_state_ = luaL_newstate();

			// smuggle a pointer to ourselves into the lua state's extraspace
			// this way, we can find the c++ class from c function callbacks
			*((Bot **) lua_getextraspace(lua_state_)) = this;

			lua_atpanic(lua_state_, Bot_HandleError);
			lua_setwarnf(lua_state_, Bot_HandleWarning, lua_state_);

			luaL_openlibs(lua_state_);

			luaL_loadstring(lua_state_, "warn('test');print('test2')");
			if (setjmp(preexec_state) == 0)
			{
				lua_call(lua_state_, 0, 0);
			}
		}

		Bot::~Bot()
		{
			lua_close(lua_state_);
		}

		void Bot::HandleWarning_(const char * msg)
		{
			printf("WARNING: %s\n", msg);
		}

		void Bot::HandleError_()
		{
			const char *msg = lua_tostring(lua_state_, -1);

			if (msg == NULL) msg = "error object is not a string";
			printf("ERROR: unprotected error in call to Lua API (%s)\n", msg);

			longjmp(preexec_state, 0);
		}
	}
}