#ifndef _GAME_HPP
#define _GAME_HPP

#include <string>

#include <SDL.h>

#include "content/manager.hpp"
#include "game/editor/editor_thread.hpp"
#include "game/screen.hpp"
#include "music/manager.hpp"
#include "input/manager.hpp"

namespace duckhacker
{
	namespace game
	{
		class MenuMainScreen;
		class WorldScreen;
	}

	namespace world
	{
		class World;
	}

	class Game
	{
	public:
		~Game();

		void GoToMainMenu();
		void GoToWorld(std::string world_path);

		void Run();

	private:
		void SetScreen_(game::Screen * screen);
		void HandleFatalError(const char * message);

		SDL_Window * window_;

		game::editor::EditorThread editor_thread_;

		game::Screen * current_screen_ = nullptr;

		world::World * world_ = nullptr;

		game::MenuMainScreen * menu_main_screen_ = nullptr;
		game::WorldScreen * world_screen_ = nullptr;

		content::Manager content_manager_;
		music::Manager music_manager_;
		input::Manager input_manager_;
	};
};

#endif