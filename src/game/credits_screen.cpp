#include "game/credits_screen.hpp"

#include "game.hpp"

namespace duckhacker
{
	namespace game
	{
		CreditsScreen::CreditsScreen(Game * game, content::Manager * content_manager)
		{
			game_ = game;
		}

		CreditsScreen::~CreditsScreen()
		{
		}

		void CreditsScreen::OnEnter()
		{
			// line_index_ = 0;
		}

		void CreditsScreen::Update(double dt, input::Manager * input_manager)
		{
		}

		void CreditsScreen::Draw(content::Manager * content_manager)
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));

			ImGui::Begin("Credits", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

			ImGui::PushFont(content_manager->Font(content::FontType::LARGE));

			ImGui::TextWrapped("TODO: write credits");

			ImGui::PopFont();

			ImGui::End();

			ImGui::PopStyleColor();
		}
	}
}