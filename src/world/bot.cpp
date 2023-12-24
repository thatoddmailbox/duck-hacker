#include "world/bot.hpp"

#include <sstream>

#include "world/world.hpp"

const float BOT_ANIMATION_TIME = 1;
const float BOT_PAUSE_TIME = 0.1;

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

		static int Bot_OnLuaCall_MoveForward(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Move_(1, 0, 0);
		}

		static int Bot_OnLuaCall_MoveBackward(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Move_(-1, 0, 0);
		}

		static int Bot_OnLuaCall_CanMoveUp(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_CanMove_(0, 1, 0);
		}

		static int Bot_OnLuaCall_CanMoveDown(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_CanMove_(0, -1, 0);
		}

		static int Bot_OnLuaCall_CanMoveForward(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_CanMove_(1, 0, 0);
		}

		static int Bot_OnLuaCall_CanMoveBackward(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_CanMove_(-1, 0, 0);
		}

		static int Bot_OnLuaCall_TurnLeft(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Turn_(-90);
		}

		static int Bot_OnLuaCall_TurnRight(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Turn_(90);
		}

		static int Bot_OnLuaCall_GetCoins(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_GetCoins_();
		}

		static int Bot_OnLuaCall_GetTime(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_GetTime_();
		}

		static int Bot_OnLuaCall_Sleep(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Sleep_();
		}

		static int Bot_OnLuaCall_NPC_AddCoins(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_NPC_AddCoins_();
		}

		static int Bot_OnLuaCall_NPC_Win(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_NPC_Win_();
		}

		Bot::Bot(world::World * world, content::Manager * content_manager, BotType t, int id, std::string name, int x, int y, int z, int rotation, std::string mesh, std::string c)
		{
			world_ = world;

			id_ = id;
			x_ = x;
			y_ = y;
			z_ = z;
			rotation_ = rotation;
			display_coords_ = glm::vec3(x_, y_, z_);
			display_rotation_ = rotation;

			type = t;
			name_ = name.empty() ? "DuckBot " + std::to_string(id_) : name;

			code = c.empty() ? "-- Code for " + name_ + "\n" : c;

			action_available_ = false;
			action_done_ = false;

			stop_requested_ = false;

			lua_state_ = nullptr;

			std::string mesh_path = mesh.empty() ? "models/duckbot.obj" : mesh;

			object.SetMesh(content_manager->Mesh(mesh_path, content_manager->Shader("shaders/basic")));
			object.SetPosition(glm::vec3(x_, y_, z_));
			object.SetRotation(glm::vec3(0, -rotation, 0));
		}

		Bot::~Bot()
		{
			// lua_close(lua_state_);
		}

		const int& Bot::GetID()
		{
			return id_;
		}

		const std::string& Bot::GetName()
		{
			return name_;
		}

		const int& Bot::GetX()
		{
			return x_;
		}

		const int& Bot::GetY()
		{
			return y_;
		}

		const int& Bot::GetZ()
		{
			return z_;
		}

		const int& Bot::GetRotation()
		{
			return rotation_;
		}

		void Bot::ResetTo(int x, int y, int z, int rotation)
		{
			if (running_)
			{
				std::cout << "Cannot reset a bot while it is running." << std::endl;
				return;
			}

			x_ = x;
			y_ = y;
			z_ = z;
			rotation_ = rotation;
			display_coords_ = glm::vec3(x_, y_, z_);
			display_rotation_ = rotation;

			object.SetPosition(glm::vec3(x_, y_, z_));
			object.SetRotation(glm::vec3(0, -rotation, 0));
		}

		const glm::vec3& Bot::GetDisplayCoords()
		{
			return display_coords_;
		}

		const float& Bot::GetDisplayRotation()
		{
			return display_rotation_;
		}

		const std::atomic_bool& Bot::IsCrashed()
		{
			return crashed_;
		}

		void Bot::Log(ConsoleLineType line_type, std::string line)
		{
			std::unique_lock<std::mutex> lock(lines_mutex_);

			std::stringstream ss(line);
			std::string individual_line;
			while (std::getline(ss, individual_line, '\n'))
			{
				ConsoleLine console_line;

				console_line.ticks = world_->GetTicks();
				console_line.type = line_type;
				console_line.text = individual_line;
				console_line.name = name_;

				lines_.push_back(console_line);
			}
		}

		void Bot::Execute()
		{
			execute_thread_ = std::thread(Bot::EnterExecuteThread_, this);
			running_ = true;
			crashed_ = false;
		}

		void Bot::EnterExecuteThread_(Bot * b)
		{
			b->Execute_();
		}

		int Bot::OnLuaCall_Move_(int dx, int dy, int dz)
		{
			int n = lua_gettop(lua_state_);
			if (n != 0)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			// check that we can move there
			OnLuaCall_CanMove_(dx, dy, dz);
			bool can_move = lua_toboolean(lua_state_, -1);
			lua_pop(lua_state_, 1);
			if (!can_move)
			{
				lua_pushliteral(lua_state_, "can't move there (something is in the way)");
				return lua_error(lua_state_);
			}

			// set up the action
			printf("move %d %d %d\n", dx, dy, dz);
			action_done_ = false;
			action_type_ = BotAction::MOVE;
			action_coords_[0] = dx;
			action_coords_[1] = dy;
			action_coords_[2] = dz;

			{
				std::unique_lock<std::mutex> lock(action_done_mutex_);

				// signal that something's available
				action_available_ = true;

				// wait for the action to be done
				action_done_condition_.wait(lock, [this] {
					return this->action_done_ || this->stop_requested_;
				});
			}

			if (stop_requested_)
			{
				longjmp(preexec_state, 1);
			}

			return 0;
		}

		int Bot::OnLuaCall_CanMove_(int dx, int dy, int dz)
		{
			int n = lua_gettop(lua_state_);
			if (n != 0)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			int dx_rotated = dx;
			int dy_rotated = dy;
			int dz_rotated = dz;

			if (rotation_ == 0)
			{
				// do nothing
			}
			else if (rotation_ == 90)
			{
				int temp = dx_rotated;
				dx_rotated = -dz_rotated;
				dz_rotated = temp;
			}
			else if (rotation_ == 180)
			{
				dx_rotated = -dx_rotated;
				dz_rotated = -dz_rotated;
			}
			else if (rotation_ == 270)
			{
				int temp = dx_rotated;
				dx_rotated = dz_rotated;
				dz_rotated = -temp;
			}

			int x = x_ + dx_rotated;
			int y = y_ + dy_rotated;
			int z = z_ + dz_rotated;

			bool result = !world_->IsOccupied(x, y, z);
			lua_pushboolean(lua_state_, result);
			return 1;
		}

		int Bot::OnLuaCall_Turn_(int da)
		{
			int n = lua_gettop(lua_state_);
			if (n != 0)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			// set up the action
			printf("turn %d\n", da);
			action_done_ = false;
			action_type_ = BotAction::TURN;
			action_angle_ = da;

			{
				std::unique_lock<std::mutex> lock(action_done_mutex_);

				// signal that something's available
				action_available_ = true;

				// wait for the action to be done
				action_done_condition_.wait(lock, [this] {
					return this->action_done_ || this->stop_requested_;
				});
			}

			if (stop_requested_)
			{
				longjmp(preexec_state, 1);
			}

			return 0;
		}

		int Bot::OnLuaCall_GetCoins_()
		{
			int n = lua_gettop(lua_state_);
			if (n != 0)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			lua_pushinteger(lua_state_, world_->GetCoins());
			return 1;
		}

		int Bot::OnLuaCall_GetTime_()
		{
			int n = lua_gettop(lua_state_);
			if (n != 0)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			lua_pushinteger(lua_state_, world_->GetTicks());
			return 1;
		}

		int Bot::OnLuaCall_Sleep_()
		{
			int n = lua_gettop(lua_state_);
			if (n != 1)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			int is_num = 0;
			int ticks = lua_tointegerx(lua_state_, 1, &is_num);
			if (!is_num)
			{
				lua_pushliteral(lua_state_, "sleep time must be an integer");
				return lua_error(lua_state_);
			}
			else if (ticks <= 0)
			{
				lua_pushliteral(lua_state_, "sleep time must be positive");
				return lua_error(lua_state_);
			}

			int start_time = world_->GetTicks();
			while (world_->GetTicks() - start_time < ticks)
			{
				// TODO: kinda sus that we're using SDL in a non-main thread
				// it's fine for the platforms we care about (windows and linux) but it's not guaranteed to work
				SDL_Delay(100);

				if (stop_requested_)
				{
					longjmp(preexec_state, 1);
				}
			}

			return 0;
		}

		int Bot::OnLuaCall_NPC_AddCoins_()
		{
			int n = lua_gettop(lua_state_);
			if (n != 1)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			int is_num = 0;
			int amount = lua_tointegerx(lua_state_, 1, &is_num);
			if (!is_num)
			{
				lua_pushliteral(lua_state_, "amount must be an integer");
				return lua_error(lua_state_);
			}

			world_->AddCoins(amount);

			return 0;
		}

		int Bot::OnLuaCall_NPC_Win_()
		{
			int n = lua_gettop(lua_state_);
			if (n != 0)
			{
				lua_pushliteral(lua_state_, "incorrect number of arguments");
				return lua_error(lua_state_);
			}

			action_done_ = false;
			action_type_ = BotAction::WIN;

			{
				std::unique_lock<std::mutex> lock(action_done_mutex_);

				// signal that something's available
				action_available_ = true;

				// wait for the action to be done
				action_done_condition_.wait(lock, [this] {
					return this->action_done_ || this->stop_requested_;
				});
			}

			if (stop_requested_)
			{
				longjmp(preexec_state, 1);
			}

			return 0;
		}

		void Bot::RequestStop()
		{
			stop_requested_ = true;
			action_done_condition_.notify_all();
		}

		void Bot::WaitForStop()
		{
			execute_thread_.join();
			running_ = false;
			crashed_ = false;

			// reset internal state
			action_available_ = false;
			action_done_ = false;
			anim_counter_ = 0;
			anim_happening_ = false;
			stop_requested_ = false;
		}

		static void Bot_HandleInstructionCount(lua_State * L, lua_Debug * ar)
		{
			(*((Bot **) lua_getextraspace(L)))->HandleInstructionCount_();
		}

		static int build_traceback(lua_State * L)
		{
			// from lua source code
			const char *msg = lua_tostring(L, 1);
			if (msg == NULL) {  /* is error object not a string? */
				if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
					lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
				return 1;  /* that is the message */
				else
				msg = lua_pushfstring(L, "(error object is a %s value)",
										luaL_typename(L, 1));
			}
			luaL_traceback(L, L, msg, 2);  /* append a standard traceback */
			return 1;  /* return the traceback */
		}

		static int Bot_OnLuaCall_Print(lua_State * L)
		{
			// based on lua source code
			// see https://github.com/lua/lua/blob/7923dbbf72da303ca1cca17efd24725668992f15/lbaselib.c#L24-L37

			std::string output;

			int n = lua_gettop(L);  /* number of arguments */
			int i;
			for (i = 1; i <= n; i++) {  /* for each argument */
				size_t l;
				const char *s = luaL_tolstring(L, i, &l);  /* convert it to string */
				if (i > 1)  /* not the first element? */
					output += "\t";
				output += std::string(s, l);  /* print it */
				lua_pop(L, 1);  /* pop result */
			}
			output += "\n";

			(*((Bot **) lua_getextraspace(L)))->Log(ConsoleLineType::INFO, output);
			return 0;
		}

		// this is a copy of the structure from linit.c
		// we omit several libraries that we don't want
		static const luaL_Reg loadedlibs[] = {
			{LUA_GNAME, luaopen_base},
			{LUA_TABLIBNAME, luaopen_table},
			{LUA_STRLIBNAME, luaopen_string},
			{LUA_MATHLIBNAME, luaopen_math},
			{LUA_UTF8LIBNAME, luaopen_utf8},
			{NULL, NULL}
		};

		void Bot::Execute_()
		{
			lua_state_ = luaL_newstate();

			// smuggle a pointer to ourselves into the lua state's extraspace
			// this way, we can find the c++ class from c function callbacks
			*((Bot **) lua_getextraspace(lua_state_)) = this;

			lua_atpanic(lua_state_, Bot_HandleError);
			lua_setwarnf(lua_state_, Bot_HandleWarning, lua_state_);

			// as per linit.c
			const luaL_Reg *lib;
			for (lib = loadedlibs; lib->func; lib++) {
				luaL_requiref(lua_state_, lib->name, lib->func, 1);
				lua_pop(lua_state_, 1);
			}

			// we also want to remove some problematic functions from the global table
			// we also replace print with our own implementation that logs to our console instead of stdout
			lua_getglobal(lua_state_, LUA_GNAME);

			lua_pushnil(lua_state_);
			lua_setfield(lua_state_, -2, "dofile");

			lua_pushnil(lua_state_);
			lua_setfield(lua_state_, -2, "loadfile");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_Print);
			lua_setfield(lua_state_, -2, "print");

			lua_pop(lua_state_, 1);

			lua_sethook(lua_state_, Bot_HandleInstructionCount, LUA_MASKCOUNT, 100);

			//
			// set up native functions
			//

			lua_newtable(lua_state_);

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveUp);
			lua_setfield(lua_state_, 1, "moveUp");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveDown);
			lua_setfield(lua_state_, 1, "moveDown");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveForward);
			lua_setfield(lua_state_, 1, "moveForward");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveBackward);
			lua_setfield(lua_state_, 1, "moveBackward");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_CanMoveUp);
			lua_setfield(lua_state_, 1, "canMoveUp");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_CanMoveDown);
			lua_setfield(lua_state_, 1, "canMoveDown");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_CanMoveForward);
			lua_setfield(lua_state_, 1, "canMoveForward");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_CanMoveBackward);
			lua_setfield(lua_state_, 1, "canMoveBackward");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_TurnLeft);
			lua_setfield(lua_state_, 1, "turnLeft");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_TurnRight);
			lua_setfield(lua_state_, 1, "turnRight");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_GetCoins);
			lua_setfield(lua_state_, 1, "getCoins");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_GetTime);
			lua_setfield(lua_state_, 1, "getTime");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_Sleep);
			lua_setfield(lua_state_, 1, "sleep");

			lua_setglobal(lua_state_, "duckbot");

			// for npcs, we have some special secret functions
			if (type == BotType::NPC)
			{
				lua_newtable(lua_state_);

				lua_pushcfunction(lua_state_, Bot_OnLuaCall_NPC_AddCoins);
				lua_setfield(lua_state_, 1, "addCoins");

				lua_pushcfunction(lua_state_, Bot_OnLuaCall_NPC_Win);
				lua_setfield(lua_state_, 1, "win");

				lua_setglobal(lua_state_, "npc");
			}

			lua_pushcfunction(lua_state_, build_traceback);
			int traceback_idx = lua_gettop(lua_state_);

			int status = luaL_loadbuffer(lua_state_, code.c_str(), code.length(), "=code");

			if (setjmp(preexec_state) == 0)
			{
				if (status != LUA_OK)
				{
					HandleError_();
				}

				int status = lua_pcall(lua_state_, 0, 0, traceback_idx);
				if (status != LUA_OK)
				{
					HandleError_();
				}
			}

			lua_close(lua_state_);
			lua_state_ = nullptr;
			printf("execute thread terminated :O\n");
		}

		void Bot::Update(float dt)
		{
			if (action_available_)
			{
				action_available_ = false;

				if (action_type_ == BotAction::MOVE)
				{
					int dx_rotated = action_coords_[0];
					int dy_rotated = action_coords_[1];
					int dz_rotated = action_coords_[2];

					if (rotation_ == 0)
					{
						// do nothing
					}
					else if (rotation_ == 90)
					{
						int temp = dx_rotated;
						dx_rotated = -dz_rotated;
						dz_rotated = temp;
					}
					else if (rotation_ == 180)
					{
						dx_rotated = -dx_rotated;
						dz_rotated = -dz_rotated;
					}
					else if (rotation_ == 270)
					{
						int temp = dx_rotated;
						dx_rotated = dz_rotated;
						dz_rotated = -temp;
					}

					target_x_ = x_ + dx_rotated;
					target_y_ = y_ + dy_rotated;
					target_z_ = z_ + dz_rotated;
					target_rotation_ = rotation_;
					target_rotation_display_ = rotation_;
					anim_counter_ = 0;
					anim_happening_ = true;
				}
				else if (action_type_ == BotAction::TURN)
				{
					target_x_ = x_;
					target_y_ = y_;
					target_z_ = z_;

					target_rotation_ = rotation_ + action_angle_;
					target_rotation_display_ = target_rotation_;
					if (target_rotation_ < 0)
					{
						target_rotation_ += 360;
					}
					if (target_rotation_ >= 360)
					{
						target_rotation_ -= 360;
					}

					anim_counter_ = 0;
					anim_happening_ = true;
				}
				else if (action_type_ == BotAction::WIN)
				{
					world_->Stop();
					world_->Win();

					action_done_mutex_.lock();
					action_done_ = true;
					action_done_mutex_.unlock();
					action_done_condition_.notify_one();
				}
			}

			if (anim_happening_)
			{
				anim_counter_ += dt;

				float fraction = anim_counter_ / (BOT_ANIMATION_TIME - BOT_PAUSE_TIME);
				if (fraction > 1)
				{
					fraction = 1;
				}

				display_coords_ = glm::mix(
					glm::vec3(x_, y_, z_),
					glm::vec3(target_x_, target_y_, target_z_),
					fraction
				);
				display_rotation_ = glm::mix(
					rotation_,
					target_rotation_display_,
					fraction
				);

				object.SetPosition(display_coords_);
				object.SetRotation(glm::vec3(0, -display_rotation_, 0));

				if (anim_counter_ > BOT_ANIMATION_TIME)
				{
					// we're done!
					x_ = target_x_;
					y_ = target_y_;
					z_ = target_z_;
					display_coords_ = glm::vec3(x_, y_, z_);

					rotation_ = target_rotation_;
					display_rotation_ = rotation_;

					// notify execute thread
					action_done_mutex_.lock();
					action_done_ = true;
					action_done_mutex_.unlock();
					action_done_condition_.notify_one();
				}
			}
		}

		void Bot::HandleWarning_(const char * msg)
		{
			Log(ConsoleLineType::WARNING, std::string(msg));
		}

		void Bot::HandleError_()
		{
			const char * msg = lua_tostring(lua_state_, -1);
			if (msg == NULL)
			{
				msg = "error object is not a string";
			}
\
			Log(ConsoleLineType::ERROR, std::string(msg));

			crashed_ = true;

			longjmp(preexec_state, 1);
		}

		void Bot::HandleInstructionCount_()
		{
			if (stop_requested_)
			{
				longjmp(preexec_state, 1);
			}
		}
	}
}