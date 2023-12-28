#ifndef _GAME_MENU_MAIN_SCREEN_HPP
#define _GAME_MENU_MAIN_SCREEN_HPP

#include "defs.hpp"

#include "content/manager.hpp"
#include "game/screen.hpp"
#include "input/manager.hpp"

namespace duckhacker
{
	class Game;

	namespace game
	{
		class MainMenuScreen : public Screen
		{
		public:
			MainMenuScreen(Game * game, content::Manager * content_manager);
			~MainMenuScreen();

			void Update(double dt, input::Manager * input_manager) override;
			void Draw(content::Manager * content_manager) override;

		private:
			Game * game_;
		};
	}
}

#endif