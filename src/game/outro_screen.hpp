#ifndef _GAME_OUTRO_SCREEN_HPP
#define _GAME_OUTRO_SCREEN_HPP

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
		struct OutroLine
		{
			std::string speaker;
			std::string text;
		};

		class OutroScreen : public Screen
		{
		public:
			OutroScreen(Game * game, content::Manager * content_manager);
			~OutroScreen();

			void OnEnter();
			void Update(double dt, input::Manager * input_manager) override;
			void Draw(content::Manager * content_manager) override;

		private:
			Game * game_;

			std::vector<OutroLine> lines_;
			int line_index_;
		};
	}
}

#endif