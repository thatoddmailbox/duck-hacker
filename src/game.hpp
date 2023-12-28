#ifndef _GAME_HPP
#define _GAME_HPP

#include <SDL.h>

#include "content/manager.hpp"
#include "game/screen.hpp"
#include "music/manager.hpp"
#include "input/manager.hpp"

namespace duckhacker
{
	class Game
	{
	public:
		void SetScreen(game::Screen * screen);
		void Run();

	private:
		void HandleFatalError(const char * message);

		SDL_Window * window_;

		game::Screen * current_screen_ = nullptr;
		content::Manager content_manager_;
		music::Manager music_manager_;
		input::Manager input_manager_;
	};
};

#endif