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
			game_->GetProgressManager().SetLevelCompleted("credits");
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
				"Created by Alex Studer\n"
				"\n"
				"Font:\n"
				"\tCopyright (c) 2010-2015 by tyPoland Lukasz Dziedzic with Reserved Font Name \"Lato\".\n"
				"\tLicensed under the SIL Open Font License, Version 1.1.\n"
				"\n"
				"Music:\n"
				"\t- Adding the Sun, Kevin MacLeod (incompetech.com)\n"
				"\t- Ethernight Club, Kevin MacLeod (incompetech.com)\n"
				"\t- Jingle_Win_Synth_01, https://freesound.org/people/LittleRobotSoundFactory/sounds/274179/\n"
				"\t- Space 1990, Kevin MacLeod (incompetech.com)\n"
				"\tAll tracks licensed under Creative Commons: By Attribution 4.0 License\n"
				"\thttp://creativecommons.org/licenses/by/4.0/\n"
				"\n"
				"Open-source software used:\n"
				"\t- dear imgui: Copyright (c) 2014-2023 Omar Cornut\n"
				"\t- glad: Copyright (c) 2013-2021 David Herberth\n"
				"\t- glm: Copyright (c) 2005 G-Truc Creation\n"
				"\t- libpng: Copyright (c) 1995-2023 The PNG Reference Library Authors\n"
				"\t- lua: Copyright (c) 1994-2023 Lua.org, PUC-Rio\n"
				"\t- pugixml: Copyright (c) 2006-2023 Arseny Kapoulkine\n"
				"\t- SDL: Copyright (C) 1997-2023 Sam Lantinga <slouken@libsdl.org>\n"
				"\t- SDL_mixer: Copyright (C) 1997-2023 Sam Lantinga <slouken@libsdl.org>\n"
				"\t- PhysicsFS: Copyright (c) 2001-2022 Ryan C. Gordon <icculus@icculus.org> and others.\n"
				"\n"
				"Thanks for playing!\n"
			);

			ImGui::PopFont();

			constexpr int BUTTON_WIDTH = 200;
			constexpr int BUTTON_HEIGHT = 50;

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - BUTTON_WIDTH) / 2.0f);
			ImGui::SetCursorPosY(SCREEN_HEIGHT - 100);
			if (ImGui::Button("Back to main menu", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
			{
				game_->GoToMainMenu();
			}

			ImGui::End();

			ImGui::PopStyleColor();
		}
	}
}