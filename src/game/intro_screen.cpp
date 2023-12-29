#include "game/intro_screen.hpp"

#include "game.hpp"

static std::vector<std::vector<std::string>> all_lines_ =
{
	{
		"The year is 2041.",
		"",
		"Duck society has evolved to the point where ducks use small robots called DuckBots.",
		"",
		"DuckBots perform all sorts of tasks, leaving ducks with more time to do what they want.",
		"",
		"This has led to a golden age of duck civilization.",
	},
	{
		"One day, a duck named Featherwing had an extremely important message to get to Ducky.",
		"",
		"(Ducky was the leader of the duck civilization.)",
		"",
		"The problem was that Ducky is very busy, and Featherwing couldn't get an appointment.",
		"",
		"Featherwing decided to take matters into his own hands.",
	},
	{
		"He knew that the best way to get to Ducky was to become a grasshopper deliveryman.",
		"",
		"Doing so would allow him to get into Ducky's office and give him the message.",
		"",
		"The problem was that it is very difficult to get the qualifications needed for this job.",
	},
	{
		"It turns out that DuckBots are reprogrammable, although very few know how to do it.",
		"",
		"Featherwing had gotten possession of a DuckBot Programmer.",
		"",
		"He didn't know how to use it, but that's where he hoped you could help.",
	}
};

namespace duckhacker
{
	namespace game
	{
		IntroScreen::IntroScreen(Game * game, content::Manager * content_manager)
		{
			game_ = game;
		}

		IntroScreen::~IntroScreen()
		{
		}

		void IntroScreen::OnEnter()
		{
			lines_ = &all_lines_[0];
			lines_index_ = 0;
		}

		void IntroScreen::Update(double dt, input::Manager * input_manager)
		{
		}

		static void TextCentered(const char * text)
		{
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2.0f);
			ImGui::Text("%s", text);
		}

		void IntroScreen::Draw(content::Manager * content_manager)
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));

			ImGui::Begin("Intro", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

			ImGui::PushFont(content_manager->Font(content::FontType::LARGE));

			ImVec2 text_size = ImGui::CalcTextSize(lines_->at(0).c_str());
			int line_count = lines_->size();

			ImGuiStyle& style = ImGui::GetStyle();

			ImGui::SetCursorPosY((ImGui::GetWindowSize().y - ((text_size.y + style.ItemSpacing.y) * line_count)) / 2.0f);

			for (std::string line : *lines_)
			{
				TextCentered(line.c_str());
			}

			ImGui::PopFont();

			constexpr int BUTTON_WIDTH = 30;
			constexpr int BUTTON_HEIGHT = 50;

			if (lines_index_ > 0)
			{
				ImGui::SetCursorPosX(25);
				ImGui::SetCursorPosY((SCREEN_HEIGHT - BUTTON_HEIGHT) / 2);
				if (ImGui::Button("<", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
				{
					lines_index_--;
					lines_ = &all_lines_[lines_index_];
				}
			}

			ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 25 - BUTTON_WIDTH);
			ImGui::SetCursorPosY((SCREEN_HEIGHT - BUTTON_HEIGHT) / 2);
			if (ImGui::Button(">", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
			{
				if (lines_index_ == all_lines_.size() - 1)
				{
					game_->GoToWorld("worlds/level1.xml");
				}

				lines_index_++;
				lines_ = &all_lines_[lines_index_];
			}

			ImGui::End();

			ImGui::PopStyleColor();
		}
	}
}