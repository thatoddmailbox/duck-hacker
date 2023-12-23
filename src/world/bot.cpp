#include "world/bot.hpp"

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

		static int Bot_OnLuaCall_MoveLeft(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Move_(0, 0, -1);
		}

		static int Bot_OnLuaCall_MoveRight(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Move_(0, 0, 1);
		}

		static int Bot_OnLuaCall_TurnLeft(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Turn_(-90);
		}

		static int Bot_OnLuaCall_TurnRight(lua_State * L)
		{
			return (*((Bot **) lua_getextraspace(L)))->OnLuaCall_Turn_(90);
		}

		Bot::Bot(content::Manager * content_manager, int id, int x, int y, int z, int rotation)
		{
			id_ = id;
			x_ = x;
			y_ = y;
			z_ = z;
			rotation_ = rotation;
			display_coords_ = glm::vec3(x_, y_, z_);
			display_rotation_ = rotation;

			code = "-- Code for DuckBot " + std::to_string(id_) + "\n";

			action_available_ = false;
			action_done_ = false;

			lua_state_ = nullptr;

			object.SetMesh(content_manager->Mesh("models/duckbot.obj", content_manager->Shader("shaders/gray")));
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

		const glm::vec3& Bot::GetDisplayCoords()
		{
			return display_coords_;
		}

		const float& Bot::GetDisplayRotation()
		{
			return display_rotation_;
		}

		void Bot::Execute()
		{
			execute_thread_ = std::thread(Bot::EnterExecuteThread_, this);
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
					return this->action_done_;
				});
			}

			return 0;
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

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveForward);
			lua_setfield(lua_state_, 1, "moveForward");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveBackward);
			lua_setfield(lua_state_, 1, "moveBackward");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveLeft);
			lua_setfield(lua_state_, 1, "moveLeft");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_MoveRight);
			lua_setfield(lua_state_, 1, "moveRight");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_TurnLeft);
			lua_setfield(lua_state_, 1, "turnLeft");

			lua_pushcfunction(lua_state_, Bot_OnLuaCall_TurnRight);
			lua_setfield(lua_state_, 1, "turnRight");

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
					target_x_ = x_ + action_coords_[0];
					target_y_ = y_ + action_coords_[1];
					target_z_ = z_ + action_coords_[2];
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