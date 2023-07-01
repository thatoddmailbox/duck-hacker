#ifndef _GAME_SCREEN_HPP
#define _GAME_SCREEN_HPP

#include <SDL.h>

namespace duckhacker
{
	namespace game
	{
		class Screen
		{
		public:
			virtual void Update(double dt) = 0;
			virtual void Draw(content::Manager * content_manager) = 0;
		};
	};
};

#endif