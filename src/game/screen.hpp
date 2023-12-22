#ifndef _GAME_SCREEN_HPP
#define _GAME_SCREEN_HPP

#include "content/manager.hpp"
#include "input/manager.hpp"

namespace duckhacker
{
	namespace game
	{
		class Screen
		{
		public:
			virtual void Update(double dt, input::Manager * input_manager) = 0;
			virtual void Draw(content::Manager * content_manager) = 0;
		};
	};
};

#endif