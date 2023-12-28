#include "game/menu_main_screen.hpp"

#include <SDL.h>

#include "game.hpp"
#include "game/world_screen.hpp"

namespace duckhacker
{
	namespace game
	{
		MenuMainScreen::MenuMainScreen(Game * game, content::Manager * content_manager)
		{
			game_ = game;
		}

		MenuMainScreen::~MenuMainScreen()
		{
		}

		void MenuMainScreen::Update(double dt, input::Manager * input_manager)
		{
		}

		static void TextCentered(const char * text)
		{
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2.0f);
			ImGui::Text("%s", text);
		}

		void MenuMainScreen::Draw(content::Manager * content_manager)
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));

			ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

			ImGui::SetCursorPosY(100);
			ImGui::PushFont(content_manager->Font(content::FontType::TITLE));
			TextCentered("Duck Hacker");
			ImGui::PopFont();

			constexpr int BUTTON_WIDTH = 200;
			constexpr int BUTTON_HEIGHT = 50;

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - BUTTON_WIDTH) / 2.0f);
			ImGui::SetCursorPosY(200);
			if (ImGui::Button("Play", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
			{
				world::World * world = new world::World(content_manager, "worlds/level4.xml");
				game_->SetScreen(new game::WorldScreen(content_manager, nullptr, world));
			}

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - BUTTON_WIDTH) / 2.0f);
			ImGui::SetCursorPosY(200 + BUTTON_HEIGHT + 20);
			if (ImGui::Button("Quit", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
			{
				SDL_Event event;
				event.type = SDL_QUIT;
				SDL_PushEvent(&event);
			}

			ImGui::End();

			ImGui::PopStyleColor();
		}
	}
}