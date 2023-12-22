#ifndef _GAME_HPP
#define _GAME_HPP

#include <SDL.h>

#include "game/screen.hpp"

namespace duckhacker
{
	class Game
	{
	public:
		void Run();

	private:
		void HandleFatalError(const char * message);

		SDL_Window * window_;

		game::Screen * current_screen_ = nullptr;
		content::Manager content_manager_;
		input::Manager input_manager_;
	};
};

#endif