#include "game.hpp"

#include <iostream>
#include <thread>

#include <physfs.h>

#include "glad/glad.h"

#include "external/imgui/imgui.h"
#include "external/imgui/backends/imgui_impl_sdl2.h"
#include "external/imgui/backends/imgui_impl_opengl3.h"

#include "defs.hpp"
#include "game/credits_screen.hpp"
#include "game/intro_screen.hpp"
#include "game/level_select_screen.hpp"
#include "game/main_menu_screen.hpp"
#include "game/outro_screen.hpp"
#include "game/world_screen.hpp"
#include "world/world.hpp"

namespace duckhacker
{
	Game::~Game()
	{
		if (level_select_screen_)
		{
			delete level_select_screen_;
		}
		if (main_menu_screen_)
		{
			delete main_menu_screen_;
		}
		if (world_screen_)
		{
			delete world_screen_;
		}
		if (world_)
		{
			delete world_;
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		PHYSFS_deinit();
	}

	void Game::HandleFatalError(const char * message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message, NULL);
	}

	void Game::SetScreen_(game::Screen * screen)
	{
		current_screen_ = screen;
	}

	void Game::GoToCredits()
	{
		if (!credits_screen_)
		{
			credits_screen_ = new game::CreditsScreen(this, &content_manager_);
		}

		credits_screen_->OnEnter();
		music_manager_.PlayTrack("outro");
		SetScreen_(credits_screen_);
	}

	void Game::GoToIntro()
	{
		if (!intro_screen_)
		{
			intro_screen_ = new game::IntroScreen(this, &content_manager_);
		}

		intro_screen_->OnEnter();
		music_manager_.PlayTrack("menu");
		SetScreen_(intro_screen_);
	}

	void Game::GoToLevelSelect()
	{
		if (world_screen_)
		{
			delete world_screen_;
			world_screen_ = nullptr;
		}
		if (world_)
		{
			delete world_;
			world_ = nullptr;
		}

		if (!level_select_screen_)
		{
			level_select_screen_ = new game::LevelSelectScreen(this, &content_manager_);
		}
		level_select_screen_->OnEnter();
		music_manager_.PlayTrack("menu");
		SetScreen_(level_select_screen_);
	}

	void Game::GoToMainMenu()
	{
		if (world_screen_)
		{
			delete world_screen_;
			world_screen_ = nullptr;
		}
		if (world_)
		{
			delete world_;
			world_ = nullptr;
		}

		if (!main_menu_screen_)
		{
			main_menu_screen_ = new game::MainMenuScreen(this, &content_manager_);
		}
		music_manager_.PlayTrack("menu");
		SetScreen_(main_menu_screen_);
	}

	void Game::GoToOutro()
	{
		editor_thread_.SetWorld(nullptr);

		if (!outro_screen_)
		{
			outro_screen_ = new game::OutroScreen(this, &content_manager_);
		}

		outro_screen_->OnEnter();
		music_manager_.PlayTrack("outro");
		SetScreen_(outro_screen_);
	}

	void Game::GoToWorld(std::string world_path)
	{
		editor_thread_.SetWorld(nullptr);

		if (world_)
		{
			delete world_;
			world_ = nullptr;
		}
		world_ = new world::World(&content_manager_, world_path);

		if (!world_screen_)
		{
			world_screen_ = new game::WorldScreen(this, &content_manager_, &editor_thread_, world_);
		}
		else
		{
			world_screen_->SetWorld(world_);
		}
		music_manager_.PlayTrack("game");
		SetScreen_(world_screen_);
	}

	void Game::Run()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			HandleFatalError("SDL_Init failed.");
			return;
		}

		progress_manager_.Init();

		// note that windows requires multisampling settings to be set before the window is created
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		window_ = SDL_CreateWindow(
			"Duck Hacker",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
		);
		if (window_ == nullptr)
		{
			HandleFatalError("SDL_CreateWindow failed.");
			return;
		}

		//
		// set up opengl
		//

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GLContext context = SDL_GL_CreateContext(window_);

		if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		{
			HandleFatalError("Failed to start OpenGL.");
			return;
		}

		std::cout << glGetString(GL_VERSION) << std::endl;

		glEnable(GL_MULTISAMPLE);

		//
		// set up physfs
		//

		// add both the zip archive and the folder
		// this will try the folder first, then the archive
		int err = PHYSFS_mount("data.zip", nullptr, 0);
		int err2 = PHYSFS_mount("data/", nullptr, 0);
		if (err == 0 && err2 == 0)
		{
			// could not load either the zip or the folder
			HandleFatalError("Could not load data.");
			return;
		}

		//
		// set up audio
		//
		music_manager_.Init(&content_manager_);

		//
		// launch editor thread
		//
		std::thread t(game::editor::EditorThread::Run, &editor_thread_);

		GoToMainMenu();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.IniFilename = nullptr;

		ImGui::StyleColorsLight();

		content_manager_.LoadFonts();

		ImGui_ImplOpenGL3_Init();
		ImGui_ImplSDL2_InitForOpenGL(window_, context);

		bool running = true;
		bool show_imgui_demo = false;

		uint64_t last;
		uint64_t now = SDL_GetPerformanceCounter();
		double perf_freq = (double) SDL_GetPerformanceFrequency();
		double dt;

		glEnable(GL_DEPTH_TEST);

		SDL_Event e;
		while (running)
		{
			last = now;
			now = SDL_GetPerformanceCounter();
			dt = (now - last) / perf_freq;

			input_manager_.EarlyUpdate();

			while (SDL_PollEvent(&e))
			{
				input_manager_.ProcessEvent(&e);
				ImGui_ImplSDL2_ProcessEvent(&e);

				if (e.type == SDL_QUIT)
				{
					running = false;
					break;
				}

				if (e.type == SDL_KEYUP && !io.WantCaptureKeyboard)
				{
					if (e.key.keysym.sym == SDLK_BACKQUOTE)
					{
						if (progress_manager_.IsDebuggingEnabled())
						{
							show_imgui_demo = true;
						}
					}
				}
			}

			input_manager_.Update();
			if (current_screen_ != nullptr)
			{
				current_screen_->Update(dt, &input_manager_);
			}

			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			if (show_imgui_demo)
			{
				ImGui::ShowDemoWindow(&show_imgui_demo);
			}

			if (current_screen_ != nullptr)
			{
				current_screen_->Draw(&content_manager_);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			SDL_GL_SwapWindow(window_);

			// TODO: actual framerate locking
			SDL_Delay(15);
		}

		editor_thread_.RequestStop();
		t.join();

		SDL_DestroyWindow(window_);
	}
};