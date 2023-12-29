#ifndef _GAME_INTRO_SCREEN_HPP
#define _GAME_INTRO_SCREEN_HPP

#include <string>
#include <vector>

#include "defs.hpp"

#include "content/manager.hpp"
#include "game/screen.hpp"
#include "input/manager.hpp"

namespace duckhacker
{
	class Game;

	namespace game
	{
		class IntroScreen : public Screen
		{
		public:
			IntroScreen(Game * game, content::Manager * content_manager);
			~IntroScreen();

			void OnEnter();
			void Update(double dt, input::Manager * input_manager) override;
			void Draw(content::Manager * content_manager) override;

		private:
			Game * game_;

			std::vector<std::string> * lines_;
			int lines_index_;
		};
	}
}

#endif