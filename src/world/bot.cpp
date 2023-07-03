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

		static int Bot_OnLuaCall_MoveUp(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Move_(0, 1, 0);
		}

		static int Bot_OnLuaCall_MoveDown(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Move_(0, -1, 0);
		}

		Bot::Bot()
		{
			id = 0;
			x = y = z = 0;
			code = "print('duck')\n-- quack";

			action_available_ = false;
			action_done_ = false;

			lua_state_ = nullptr;
		}

		Bot::~Bot()
		{
			// lua_close(lua_state_);
		}

		void Bot::Execute()
		{
			execute_thread_ = std::thread(Bot::EnterExecuteThread_, this);
		}

		void Bot::EnterExecuteThread_(Bot * b)
		{
			b->Execute_();
		}

		int Bot::OnLuaCall_Move_(int x, int y, int z)
		{
			int n = lua_gettop(lua_state_);
			if (n != 0)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			// set up the action
			printf("move %d %d %d\n", x, y, z);
			action_done_ = false;
			action_type_ = BotAction::MOVE;
			action_coords_[0] = x;
			action_coords_[1] = y;
			action_coords_[2] = z;

			{
				std::unique_lock<std::mutex> lock(action_done_mutex_);

				// signal that something's available
				action_available_ = true;

				// wait for the action to be done
				action_done_condition_.wait(lock, [this] {
					return this->action_done_;
				});
			}

			return 0;
		}

		void Bot::RequestStop()
		{
			// TODO: this should uhhh do something
		}

		void Bot::WaitForStop()
		{
			execute_thread_.join();
		}

		void Bot::Execute_()
		{
			lua_state_ = luaL_newstate();

			// smuggle a pointer to ourselves into the lua state's extraspace
			// this way, we can find the c++ class from c function callbacks
			*((Bot **) lua_getextraspace(lua_state_)) = this;

			lua_atpanic(lua_state_, Bot_HandleError);
			lua_setwarnf(lua_state_, Bot_HandleWarning, lua_state_);

			luaL_openlibs(lua_state_);

			//
			// set up native functions
			//

			lua_newtable(lua_state_);

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveUp);
			lua_setfield(lua_state_, 1, "moveUp");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveDown);
			lua_setfield(lua_state_, 1, "moveDown");

			lua_setglobal(lua_state_, "duckbot");

			luaL_loadstring(lua_state_, code.c_str());

			if (setjmp(preexec_state) == 0)
			{
				lua_call(lua_state_, 0, 0);
			}

			lua_close(lua_state_);
			printf("execute thread terminated :O\n");
		}

		void Bot::Update(float dt)
		{
			if (action_available_)
			{
				action_available_ = false;

				if (action_type_ == BotAction::MOVE)
				{
					x += action_coords_[0];
					y += action_coords_[1];
					z += action_coords_[2];
				}

				action_done_mutex_.lock();
				action_done_ = true;
				action_done_mutex_.unlock();
				action_done_condition_.notify_one();
			}
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