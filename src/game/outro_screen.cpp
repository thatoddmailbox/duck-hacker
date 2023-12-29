#include "game/outro_screen.hpp"

#include "game.hpp"

namespace duckhacker
{
	namespace game
	{
		OutroScreen::OutroScreen(Game * game, content::Manager * content_manager)
		{
			game_ = game;

			lines_.clear();
			lines_.push_back({ "Ducky", "Congratulations!" });
			lines_.push_back({ "Ducky", "The End" });
		}

		OutroScreen::~OutroScreen()
		{
		}

		void OutroScreen::OnEnter()
		{
			line_index_ = 0;
		}

		void OutroScreen::Update(double dt, input::Manager * input_manager)
		{
			if (input_manager->WasButtonPressed(input::Button::Continue))
			{
				line_index_++;

				if (line_index_ >= lines_.size())
				{
					game_->GoToMainMenu();
				}
			}
		}

		void OutroScreen::Draw(content::Manager * content_manager)
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, SCREEN_HEIGHT - 200));
			ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, 200));

			ImGui::Begin("Conversation", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

			OutroLine& line = lines_[line_index_];

			ImGui::Text("%s", line.speaker.c_str());

			ImGui::PushFont(content_manager->Font(content::FontType::LARGE));

			ImGui::TextWrapped("%s", line.text.c_str());

			ImGui::PopFont();

			ImGui::End();
		}
	}
}