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
			lines_.push_back({ "Featherwing", "Alright. this is the moment of truth." });
			lines_.push_back({ "Featherwing", "I'm going to go talk to Ducky." });
			lines_.push_back({ "Featherwing", "I think this is the way to his office." });
			lines_.push_back({ "Featherwing", "..." });
			lines_.push_back({ "Featherwing", "Hello? It's me, the grasshopper deliveryman." });
			lines_.push_back({ "Featherwing", "..." });
			lines_.push_back({ "Featherwing", "Wait, I see him!!" });
			lines_.push_back({ "Featherwing", "HEY DUCKY!" });
			lines_.push_back({ "Featherwing", "*deep breath*" });
			lines_.push_back({ "Featherwing", "Here we go..." });
			lines_.push_back({ "Featherwing", "THE SKY!!" });
			lines_.push_back({ "Featherwing", "IS BLUE!" });
			lines_.push_back({ "Featherwing", "AND YOU! ARE BLUE!!" });
			lines_.push_back({ "Featherwing", "THEREFORE: YOU ARE THE SKY!" });
			lines_.push_back({ "Featherwing", "AND THE SKY IS YOU!" });
			lines_.push_back({ "Featherwing", "..." });
			lines_.push_back({ "Ducky", "Wow, I never thought of it that way." });
			lines_.push_back({ "Ducky", "I guess I'm the sky." });
			lines_.push_back({ "Ducky", "..." });
			lines_.push_back({ "Ducky", "That's pretty cool." });
			lines_.push_back({ "Ducky", "You're promoted to Chief of Grasshoppers." });
			lines_.push_back({ "Featherwing", "Wow, thank you." });
			lines_.push_back({ "Ducky", "And you, human, will also be rewarded for your help." });
			lines_.push_back({ "Ducky", "That's right - you'll get the official Medal of Duck Honor." });
			lines_.push_back({ "Ducky", "Congratulations!" });
			lines_.push_back({ "Ducky", "..." });
			lines_.push_back({ "Ducky", "OK, I'm going to go think about the sky some more." });
			lines_.push_back({ "Ducky", "See you later!" });
			lines_.push_back({ "Featherwing", "See you later, Ducky!" });
			lines_.push_back({ "", "THE END" });
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
					game_->GetProgressManager().SetLevelCompleted("outro");
					game_->GoToCredits();
				}
			}
		}

		void OutroScreen::Draw(content::Manager * content_manager)
		{
			static constexpr int SPEECH_WIDTH_MARGIN = 20;
			static constexpr int SPEECH_HEIGHT = 200;

			ImGui::SetNextWindowPos(ImVec2(SPEECH_WIDTH_MARGIN / 2, (SCREEN_HEIGHT - SPEECH_HEIGHT) / 2));
			ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH - SPEECH_WIDTH_MARGIN, SPEECH_HEIGHT));

			ImGui::Begin("Conversation", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

			OutroLine& line = lines_[line_index_];

			if (!line.speaker.empty())
			{
				ImGui::Text("%s", line.speaker.c_str());
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
			}

			ImGui::PushFont(content_manager->Font(content::FontType::LARGE));

			ImGui::TextWrapped("%s", line.text.c_str());

			ImGui::PopFont();

			ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 26);
			ImGui::Text("Press SPACE to continue");

			ImGui::End();
		}
	}
}