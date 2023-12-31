#ifndef _GAME_HPP
#define _GAME_HPP

#include <string>

#include <SDL.h>

#include "content/manager.hpp"
#include "game/editor/editor_thread.hpp"
#include "game/screen.hpp"
#include "music/manager.hpp"
#include "input/manager.hpp"
#include "progress/manager.hpp"

namespace duckhacker
{
	namespace game
	{
		class CreditsScreen;
		class IntroScreen;
		class LevelSelectScreen;
		class MainMenuScreen;
		class OutroScreen;
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

		void GoToCredits();
		void GoToIntro();
		void GoToLevelSelect();
		void GoToMainMenu();
		void GoToOutro();
		void GoToWorld(std::string world_path);

		music::Manager& GetMusicManager() { return music_manager_; }
		progress::Manager& GetProgressManager() { return progress_manager_; }

		void Run();

	private:
		void SetScreen_(game::Screen * screen);
		void HandleFatalError(const char * message);

		SDL_Window * window_;

		game::editor::EditorThread editor_thread_;

		game::Screen * current_screen_ = nullptr;

		world::World * world_ = nullptr;

		game::CreditsScreen * credits_screen_ = nullptr;
		game::IntroScreen * intro_screen_ = nullptr;
		game::LevelSelectScreen * level_select_screen_ = nullptr;
		game::MainMenuScreen * main_menu_screen_ = nullptr;
		game::OutroScreen * outro_screen_ = nullptr;
		game::WorldScreen * world_screen_ = nullptr;

		content::Manager content_manager_;
		music::Manager music_manager_;
		input::Manager input_manager_;
		progress::Manager progress_manager_;
	};
};

#endif