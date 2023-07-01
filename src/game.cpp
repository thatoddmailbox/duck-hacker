#include "game.hpp"

#include <iostream>

#include <SDL_image.h>
#include <physfs.h>

#include "glad/glad.h"

#include "external/imgui/imgui.h"
#include "external/imgui/backends/imgui_impl_sdl2.h"
#include "external/imgui/backends/imgui_impl_opengl3.h"

#include "game/world_screen.hpp"
#include "world/world.hpp"

namespace duckhacker
{
	void Game::HandleFatalError(const char * message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message, NULL);
	}

	void Game::Run()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			HandleFatalError("SDL_Init failed.");
			return;
		}

		if (IMG_Init(IMG_INIT_PNG) == 0)
		{
			HandleFatalError("IMG_Init failed.");
			return;
		}

		window_ = SDL_CreateWindow(
			"Duck Hacker",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			800,
			600,
			SDL_WINDOW_SHOWN
		);
		if (window_ == nullptr)
		{
			HandleFatalError("SDL_CreateWindow failed.");
			return;
		}

		renderer_ = SDL_CreateRenderer(window_, 0, SDL_RENDERER_PRESENTVSYNC);
		if (renderer_ == nullptr)
		{
			HandleFatalError("SDL_CreateRenderer failed.");
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

		// TODO: error checking, read from an archive
		PHYSFS_mount("data/", nullptr, 0);

		// TODO: who owns this?
		world::World * world = new world::World();

		game::WorldScreen world_screen(&content_manager_);

		current_screen_ = nullptr;//&song_screen;

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

		SDL_Event e;
		while (running)
		{
			last = now;
			now = SDL_GetPerformanceCounter();
			dt = (now - last) / perf_freq;

			while (SDL_PollEvent(&e))
			{
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
						show_imgui_demo = true;
					}
				}
			}

			if (current_screen_ != nullptr)
			{
				current_screen_->Update(dt);
			}

			SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
			SDL_RenderClear(renderer_);

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

			SDL_RenderPresent(renderer_);

			// TODO: actual framerate locking
			SDL_Delay(15);
		}

		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
	}
};