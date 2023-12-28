#ifndef _GAME_LEVEL_SELECT_SCREEN_HPP
#define _GAME_LEVEL_SELECT_SCREEN_HPP

#include "defs.hpp"

#include "content/manager.hpp"
#include "game/screen.hpp"
#include "input/manager.hpp"

namespace duckhacker
{
	class Game;

	namespace game
	{
		class LevelSelectScreen : public Screen
		{
		public:
			LevelSelectScreen(Game * game, content::Manager * content_manager);
			~LevelSelectScreen();

			void Update(double dt, input::Manager * input_manager) override;
			void Draw(content::Manager * content_manager) override;

		private:
			Game * game_;
		};
	}
}

#endif