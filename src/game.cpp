#include "game.hpp"

#include <iostream>
#include <thread>

#include <SDL_image.h>
#include <physfs.h>

#include "glad/glad.h"

#include "external/imgui/imgui.h"
#include "external/imgui/backends/imgui_impl_sdl2.h"
#include "external/imgui/backends/imgui_impl_opengl3.h"

#include "game/world_screen.hpp"
#include "game/editor/editor_thread.hpp"
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

		//
		// launch editor thread
		//
		game::editor::EditorThread editor_thread;
		std::thread t(game::editor::EditorThread::Run, &editor_thread);

		// TODO: error checking, read from an archive
		PHYSFS_mount("data/", nullptr, 0);

		// TODO: who owns this?
		world::World * world = new world::World();

		game::WorldScreen world_screen(&content_manager_, &editor_thread, world);

		current_screen_ = &world_screen;

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
						show_imgui_demo = true;
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

		editor_thread.RequestStop();
		t.join();

		SDL_DestroyWindow(window_);
	}
};