#include "game/level_select_screen.hpp"

#include <SDL.h>

#include "game.hpp"
#include "game/world_screen.hpp"

namespace duckhacker
{
	namespace game
	{
		LevelSelectScreen::LevelSelectScreen(Game * game, content::Manager * content_manager)
		{
			game_ = game;
		}

		LevelSelectScreen::~LevelSelectScreen()
		{
		}

		void LevelSelectScreen::Update(double dt, input::Manager * input_manager)
		{
		}

		static void TextCentered(const char * text)
		{
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2.0f);
			ImGui::Text("%s", text);
		}

		void LevelSelectScreen::Draw(content::Manager * content_manager)
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));

			ImGui::Begin("Level Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

			ImGui::SetCursorPosY(50);
			ImGui::PushFont(content_manager->Font(content::FontType::TITLE));
			TextCentered("Select level");
			ImGui::PopFont();

			constexpr int BUTTON_WIDTH = 200;
			constexpr int BUTTON_HEIGHT = 50;

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - BUTTON_WIDTH) / 2.0f);
			ImGui::SetCursorPosY(50 + 64 + 20);
			if (ImGui::Button("Play", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
			{
				game_->GoToWorld("worlds/level4.xml");
			}

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - BUTTON_WIDTH) / 2.0f);
			ImGui::SetCursorPosY(50 + 64 + 20 + BUTTON_HEIGHT + 20);
			if (ImGui::Button("Back to menu", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
			{
				game_->GoToMainMenu();
			}

			ImGui::End();

			ImGui::PopStyleColor();
		}
	}
}