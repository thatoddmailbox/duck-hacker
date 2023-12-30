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

			ImGui::TextWrapped(
				"Duck Hacker\n"
				"\n"
				"Created by Alex Studer\n"
				"\n"
				"Font:\n"
				"\tCopyright (c) 2010-2015 by tyPoland Lukasz Dziedzic with Reserved Font Name \"Lato\".\n"
				"\tLicensed under the SIL Open Font License, Version 1.1.\n"
				"\n"
				"Music:\n"
				"\t- Adding the Sun\n"
				"\t- Ethernight Club\n"
				"\t- Twisting\n"
				"\tAll tracks by Kevin MacLeod (incompetech.com)\n"
				"\tLicensed under Creative Commons: By Attribution 4.0 License\n"
				"\thttp://creativecommons.org/licenses/by/4.0/\n"
				"\n"
				"Open-source software used: TODO license text\n"
				"\t- dear imgui\n"
				"\t- libpng\n"
				"\t- lua\n"
				"\t- SDL2\n"
				"\t- SDL2_mixer\n"
				"\t- PhysicsFS\n"
				"\n"
				"Thanks for playing!\n"
			);

			ImGui::PopFont();

			ImGui::End();

			ImGui::PopStyleColor();
		}
	}
}