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

		void LevelSelectScreen::OnEnter()
		{
			level_strings_.clear();
			level_unlocked_.clear();

			progress::Manager& progress_manager = game_->GetProgressManager();
			for (int i = 0; i < progress_manager.GetLevelCount(); i++)
			{
				level_strings_.push_back(progress_manager.GetLevelName(i));
				level_unlocked_.push_back(progress_manager.IsLevelUnlocked(i));
			}
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
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 8.0f));

			ImGui::Begin("Level Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

			ImGui::SetCursorPosY(50);
			ImGui::PushFont(content_manager->Font(content::FontType::TITLE));
			TextCentered("Select level");
			ImGui::PopFont();

			constexpr int BUTTON_WIDTH = 200;
			constexpr int GRID_WIDTH_COUNT = 3;
			constexpr int GRID_WIDTH_SPACING = 20;
			constexpr int GRID_WIDTH_PIXELS = (BUTTON_WIDTH * GRID_WIDTH_COUNT) + (GRID_WIDTH_SPACING * (GRID_WIDTH_COUNT - 1));
			constexpr int BUTTON_HEIGHT = 50;

			float grid_x = (ImGui::GetWindowSize().x - GRID_WIDTH_PIXELS) / 2.0f;

			ImGui::SetCursorPosX(grid_x);
			ImGui::SetCursorPosY(50 + 64 + 20);
			for (int i = 0; i < level_strings_.size(); i++)
			{
				ImGui::PushID(i);
				if (level_unlocked_[i] && ImGui::Button(level_strings_[i].c_str(), ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
				{
					const std::string level_path = game_->GetProgressManager().GetLevelPath(i);
					if (level_path == "intro")
					{
						game_->GoToIntro();
					}
					else if (level_path == "outro")
					{
						game_->GoToOutro();
					}
					else
					{
						game_->GoToWorld(level_path);
					}
				}
				ImGui::PopID();

				if ((i + 1) % GRID_WIDTH_COUNT != 0)
				{
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + GRID_WIDTH_SPACING);
				}
				else
				{
					ImGui::SetCursorPosX(grid_x);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
				}
			}

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - BUTTON_WIDTH) / 2.0f);
			ImGui::SetCursorPosY(50 + 64 + 20 + (((level_strings_.size() / GRID_WIDTH_COUNT) + 1) * (BUTTON_HEIGHT + 20)) + 40);
			if (ImGui::Button("Back to main menu", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
			{
				game_->GoToMainMenu();
			}

			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
	}
}