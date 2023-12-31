#include "game/world_screen.hpp"

#include "game.hpp"
#include "render/object.hpp"

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_internal.h"

static constexpr int EXIT_MODAL_ID = 0x12345676;
static constexpr const char * EXIT_MODAL_NAME = "Are you sure?";
static constexpr int EXIT_MODAL_WIDTH = 350;
static constexpr int EXIT_BUTTON_HEIGHT = 50;

static constexpr int MENU_MODAL_ID = 0x12345677;
static constexpr const char * MENU_MODAL_NAME = "##Menu";
static constexpr int MENU_MODAL_WIDTH = 250;
static constexpr int MENU_BUTTON_HEIGHT = 50;

static constexpr int MISSION_MODAL_ID = 0x12345678;
static constexpr const char * MISSION_MODAL_NAME = "Mission";
static constexpr int MISSION_MODAL_WIDTH = 500;

static constexpr int NO_EDIT_MODAL_ID = 0x12345679;
static constexpr const char * NO_EDIT_MODAL_NAME = "Can't edit";
static constexpr int NO_EDIT_MODAL_WIDTH = 350;

static constexpr int VICTORY_MODAL_ID = 0x12345680;
static constexpr const char * VICTORY_MODAL_NAME = "Victory";
static constexpr int VICTORY_MODAL_WIDTH = 250;
static constexpr int VICTORY_BUTTON_HEIGHT = 50;

namespace duckhacker
{
	namespace game
	{
		WorldScreen::WorldScreen(Game * game, content::Manager * content_manager, game::editor::EditorThread * editor_thread, world::World * world)
		{
			game_ = game;
			editor_thread_ = editor_thread;

			SetWorld(world);

			render::Light light;
			light.Direction = glm::vec3(-1, -0.75, -1);
			light.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			light.Ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
			light.Diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
			light.Specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			lights_.push_back(light);

			render::Light light2;
			light2.Direction = glm::vec3(1, -0.75, 1);
			light2.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			light2.Ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
			light2.Diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
			light2.Specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			lights_.push_back(light2);
		}

		WorldScreen::~WorldScreen()
		{

		}

		void WorldScreen::SetWorld(world::World * world)
		{
			world_ = world;
			focus_point_ = world_->GetCenterPoint();

			no_ui_ = true;
			opened_mission_ = false;

			yaw_ = 240.0f;
			pitch_ = 22.0f;
			radius_ = 8.0f;
			target_radius_ = 8.0f;
		}

		void WorldScreen::Update(double dt, input::Manager * input_manager)
		{
			world_->Update(dt);

			/*
			 * orbit controls
			 */
			double horizontal = input_manager->GetAxis(input::Axis::Horizontal);
			double vertical = input_manager->GetAxis(input::Axis::Vertical);

			if (horizontal != 0 || vertical != 0)
			{
				yaw_ += horizontal * 0.5f;
				pitch_ += vertical * 0.5f;
				if (pitch_ > 89.0f)
				{
					pitch_ = 89.0f;
				}
				if (pitch_ < -89.0f)
				{
					pitch_ = -89.0f;
				}
			}

			double scroll = input_manager->GetAxis(input::Axis::MouseScrollY);
			if (scroll != 0)
			{
				target_radius_ -= scroll;
				if (target_radius_ < 1)
				{
					target_radius_ = 1;
				}
				if (target_radius_ > 20)
				{
					target_radius_ = 20;
				}
			}

			if (target_radius_ != radius_)
			{
				radius_ += (target_radius_ - radius_) * 0.1f;

				if (abs(target_radius_ - radius_) < 0.001f)
				{
					radius_ = target_radius_;
				}
			}

			main_camera_.SetPosition(focus_point_ + glm::vec3(
				radius_ * cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
				radius_ * sin(glm::radians(pitch_)),
				radius_ * sin(glm::radians(yaw_)) * cos(glm::radians(pitch_))
			));
			main_camera_.LookAt(focus_point_, glm::vec3(0, 1, 0));
		}

		static void ShowTooltip(const std::string& id, glm::vec2 anchor_point, const char * text)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 text_size = ImGui::CalcTextSize(text);

			int window_width = text_size.x + style.WindowPadding.x * 2;
			int window_height = text_size.y + style.WindowPadding.y * 2;

			ImGui::SetNextWindowPos(ImVec2(anchor_point.x - (window_width / 2), anchor_point.y - (window_height / 2)), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(window_width, window_height), ImGuiCond_Always);

			ImGui::Begin(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

			ImGui::Text("%s", text);

			ImGui::End();
		}

		void WorldScreen::Draw(content::Manager * content_manager)
		{
			glm::vec3 camera_position = main_camera_.GetPosition();
			lights_[0].Direction = -glm::normalize(camera_position - world_->GetCenterPoint());

			for (world::Bot * bot : world_->bots)
			{
				bot->object.Draw(main_camera_.GetProjection(), main_camera_.GetView(), &camera_position, lights_);

				if (bot->IsCrashed())
				{
					glm::vec2 screen_position = bot->object.ObjectPositionToScreenPosition(glm::vec3(0, 0, 0), main_camera_.GetProjection(), main_camera_.GetView());
					screen_position *= glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
					ShowTooltip(bot->GetName() + " crash", screen_position, "Crashed!");
				}
				else if (bot->IsListening())
				{
					glm::vec2 screen_position = bot->object.ObjectPositionToScreenPosition(glm::vec3(0, 0, 0), main_camera_.GetProjection(), main_camera_.GetView());
					screen_position *= glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
					ShowTooltip(bot->GetName() + " listen", screen_position, "Listening...");
				}
				else if (bot->IsSpeaking())
				{
					glm::vec2 screen_position = bot->object.ObjectPositionToScreenPosition(glm::vec3(0, 0.5, 0), main_camera_.GetProjection(), main_camera_.GetView());
					screen_position *= glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
					ShowTooltip(bot->GetName() + " speak", screen_position, bot->GetSpeech().c_str());
				}
			}

			for (render::Object& object : world_->objects)
			{
				object.Draw(main_camera_.GetProjection(), main_camera_.GetView(), &camera_position, lights_);
			}

			if (!opened_mission_)
			{
				opened_mission_ = true;

				ImGui::PushOverrideID(MISSION_MODAL_ID);
				ImGui::OpenPopup(MISSION_MODAL_NAME);
				ImGui::PopID();
			}

			ImGui::PushOverrideID(MISSION_MODAL_ID);
			ImGui::SetNextWindowSize(ImVec2(MISSION_MODAL_WIDTH, 0), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), ImGuiCond_Appearing, ImVec2(0.5, 0.5));
			if (ImGui::BeginPopupModal(MISSION_MODAL_NAME, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::TextWrapped("%s", world_->GetMission().c_str());

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

				ImGui::PushFont(content_manager->Font(content::FontType::MEDIUM));
				ImGui::TextWrapped("Goal: %s", world_->GetMissionGoal().c_str());
				ImGui::PopFont();

				ImGui::SetCursorPosX(MISSION_MODAL_WIDTH - 100);
				if (ImGui::Button("OK", ImVec2(100, 0)))
				{
					no_ui_ = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			ImGui::PopID();

			if (no_ui_)
			{
				return;
			}

			world::Bot * hovered_bot = nullptr;

			ImGui::SetNextWindowPos(ImVec2(50, SCREEN_HEIGHT - 250 - 50), ImGuiCond_Appearing);
			ImGui::SetNextWindowSize(ImVec2(200, 250), ImGuiCond_Appearing);
			if (ImGui::Begin("World", nullptr, ImGuiWindowFlags_NoResize))
			{
				ImGui::Text("Time: %d", world_->GetTicks().load());
				ImGui::Text("Coins: %d", world_->GetCoins().load());

				ImGui::SeparatorText("Bots");
				for (world::Bot * bot : world_->bots)
				{
					std::string label = bot->GetName();
					if (bot->IsCrashed())
					{
						label += " (crashed)";
					}
					else if (bot->IsListening())
					{
						label += " (listening)";
					}
					else if (bot->IsSpeaking())
					{
						label += " (speaking)";
					}

					if (ImGui::Selectable(label.c_str()))
					{
						if (bot->type == world::BotType::PLAYER)
						{
							editor_thread_->OpenEditor(bot);
						}
						else
						{
							ImGui::PushOverrideID(NO_EDIT_MODAL_ID);
							ImGui::OpenPopup(NO_EDIT_MODAL_NAME);
							ImGui::PopID();
						}
					}

					if (ImGui::IsItemHovered() && !bot->IsCrashed())
					{
						hovered_bot = bot;
					}
					// ImGui::Text("position %d %d %d\n", bot->GetX(), bot->GetY(), bot->GetZ());
					// ImGui::Text("rotation %d\n", bot->GetRotation());
				}

				ImGui::SeparatorText("Inventory");
				const std::map<std::string, int>& inventory = world_->LockInventory();

				if (inventory.empty())
				{
					ImGui::Text("(empty)");
				}

				for (const auto& item : inventory)
				{
					ImGui::Text("%d %s", item.second, item.first.c_str());
				}

				world_->UnlockInventory();
			}

			ImGui::End();

			if (hovered_bot != nullptr)
			{
				glm::vec2 screen_position = hovered_bot->object.ObjectPositionToScreenPosition(glm::vec3(0, 0, 0), main_camera_.GetProjection(), main_camera_.GetView());
				screen_position *= glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
				ShowTooltip(hovered_bot->GetName(), screen_position, hovered_bot->GetName().c_str());
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

			ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(240, 240, 240, 200));
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(147, 193, 247, 255));

			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 text_size = ImGui::CalcTextSize(ICON_PLAY);

			int button_width = text_size.x + style.FramePadding.x * 2;
			int button_height = text_size.y + style.FramePadding.y * 2;

			int side_buttons_width = 100;

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, button_height + style.WindowPadding.y * 2), ImGuiCond_Always);
			ImGui::Begin("Controls", nullptr,  ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration);

			if (ImGui::Button("Mission", ImVec2(side_buttons_width, 0)))
			{
				ImGui::PushOverrideID(MISSION_MODAL_ID);
				ImGui::OpenPopup(MISSION_MODAL_NAME);
				ImGui::PopID();
			}

			ImGui::SameLine();
			ImGui::SetCursorPosX((SCREEN_WIDTH - button_width) / 2);
			if (world_->GetState() == world::State::READY)
			{
				if (ImGui::Button(ICON_PLAY))
				{
					// get code map
					std::map<int, std::string> code = editor_thread_->GatherCode();

					// update bots that have new code
					for (world::Bot * bot : world_->bots)
					{
						if (code.find(bot->GetID()) == code.end())
						{
							// no code change to this bot
							continue;
						}

						bot->SetCode(code[bot->GetID()]);
					}

					// run all bots!
					world_->Run();
				}
			}
			else
			{
				if (ImGui::Button(ICON_STOP))
				{
					world_->Stop();
					world_->Reset();
				}
			}

			if (world_->GetState() == world::State::VICTORY)
			{
				ImGui::PushOverrideID(VICTORY_MODAL_ID);
				if (!ImGui::IsPopupOpen(VICTORY_MODAL_NAME, ImGuiPopupFlags_AnyPopupId))
				{
					game_->GetMusicManager().PlaySoundEffect("win");
					game_->GetProgressManager().SetLevelCompleted(world_->GetFilePath());
					ImGui::OpenPopup(VICTORY_MODAL_NAME);
				}
				ImGui::PopID();
			}

			ImGui::SameLine();
			ImGui::SetCursorPosX(SCREEN_WIDTH - side_buttons_width - style.WindowPadding.x);
			if (ImGui::Button("Menu", ImVec2(side_buttons_width, 0)))
			{
				ImGui::PushOverrideID(MENU_MODAL_ID);
				ImGui::OpenPopup(MENU_MODAL_NAME);
				ImGui::PopID();
			}

			ImGui::End();

			ImGui::PopStyleColor(2);

			ImGui::PopStyleVar(1);

			if (game_->GetProgressManager().IsDebuggingEnabled())
			{
				ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 200 - 50, 100), ImGuiCond_Appearing);
				ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Appearing);
				if (ImGui::Begin("Scene settings"))
				{
					ImGui::SeparatorText("Camera");
					ImGui::SliderFloat("Radius##CameraRadius", &target_radius_, 1, 100);
					ImGui::SliderFloat("Yaw##CameraYaw", &yaw_, 0, 360);
					ImGui::SliderFloat("Pitch##CameraPitch", &pitch_, -89, 89);
					ImGui::SliderFloat3("Focus point##CameraFocus", &focus_point_.x, -5, 5);

					ImGui::SeparatorText("Light 1");
					ImGui::SliderFloat3("Direction##Light1Direction", &lights_[0].Direction.x, -1, 1);
					ImGui::SliderFloat4("Diffuse##Light1Diffuse", &lights_[0].Diffuse.x, 0, 1);
					ImGui::SliderFloat4("Ambient##Light1Ambient", &lights_[0].Ambient.x, 0, 1);
					ImGui::SliderFloat4("Specular##Light1Specular", &lights_[0].Specular.x, 0, 1);

					ImGui::SeparatorText("Light 2");
					ImGui::SliderFloat3("Direction##Light2Direction", &lights_[1].Direction.x, -1, 1);
					ImGui::SliderFloat4("Diffuse##Light2Diffuse", &lights_[1].Diffuse.x, 0, 1);
					ImGui::SliderFloat4("Ambient##Light2Ambient", &lights_[1].Ambient.x, 0, 1);
					ImGui::SliderFloat4("Specular##Light2Specular", &lights_[1].Specular.x, 0, 1);

					// ImGui::SeparatorText("Sign");
					// float r = -world_->objects[2].GetRotation().y;
					// if (ImGui::SliderFloat("Rotation", &r, 0, 360))
					// {
					// 	world_->objects[2].SetRotation(glm::vec3(0, -r, 0));
					// }
				}

				ImGui::End();
			}

			world::Console& console = world_->GetConsole();

			// TODO: have to mess with ImGui internals to get this to work, kind of hacky
			ImGuiWindow * console_window = ImGui::FindWindowByName("Console");
			bool console_focused = false;
			if (console_window)
			{
				console_focused = (console_window == ImGui::GetCurrentContext()->NavWindow && !console_window->SkipItems);
			}

			ImGui::SetNextWindowSize(ImVec2(450, 250), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 450 - 50, SCREEN_HEIGHT - 250 - 50), ImGuiCond_Appearing);
			if (ImGui::Begin("Console", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ((console.new_lines && !console_focused) ? ImGuiWindowFlags_UnsavedDocument : 0)))
			{
				if (console_focused)
				{
					console.new_lines = false;
				}

				const std::deque<world::ConsoleLine>& lines = console.GetLines();

				for (const world::ConsoleLine& line : lines)
				{
					bool changed_color = false;
					if (line.type == world::ConsoleLineType::ERR)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
						changed_color = true;
					}
					else if (line.type == world::ConsoleLineType::WARNING)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(162, 162, 46, 255));
						changed_color = true;
					}
					else if (line.type == world::ConsoleLineType::SPOKEN)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(62, 27, 196, 255));
						changed_color = true;
					}

					ImGui::Text("[%4d] [%s] [%s] %s", line.ticks, line.name.c_str(), world::ConsoleLineTypeToString[(int) line.type], line.text.c_str());

					if (changed_color)
					{
						ImGui::PopStyleColor();
					}
				}
			}

			ImGui::End();

			ImGui::PushOverrideID(NO_EDIT_MODAL_ID);
			ImGui::SetNextWindowSize(ImVec2(NO_EDIT_MODAL_WIDTH, 0), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), ImGuiCond_Appearing, ImVec2(0.5, 0.5));
			if (ImGui::BeginPopupModal(NO_EDIT_MODAL_NAME, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::TextWrapped("You can only change the code of your DuckBot.");

				ImGui::SetCursorPosX(NO_EDIT_MODAL_WIDTH - 100);
				if (ImGui::Button("OK", ImVec2(100, 0)))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::PushOverrideID(VICTORY_MODAL_ID);
			ImGui::SetNextWindowSize(ImVec2(VICTORY_MODAL_WIDTH, 0), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), ImGuiCond_Appearing, ImVec2(0.5, 0.5));
			if (ImGui::BeginPopupModal(VICTORY_MODAL_NAME, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				static const char * hacked_text = "Hacked!";

				ImGui::PushFont(content_manager->Font(content::FontType::TITLE));
				ImVec2 hacked_text_size = ImGui::CalcTextSize(hacked_text);
				ImGui::SetCursorPosX((VICTORY_MODAL_WIDTH + 16 - hacked_text_size.x) / 2.0f);
				ImGui::Text("%s", hacked_text);
				ImGui::PopFont();

				ImGui::Spacing();

				if (ImGui::Button("Next level", ImVec2(VICTORY_MODAL_WIDTH, VICTORY_BUTTON_HEIGHT)))
				{
					world_->Reset();
					ImGui::CloseCurrentPopup();

					progress::Manager& progress_manager = game_->GetProgressManager();

					int current_index = progress_manager.GetLevelIndexFromPath(world_->GetFilePath());
					std::string next_path = progress_manager.GetLevelPath(current_index + 1);

					if (next_path == "outro")
					{
						game_->GoToOutro();
					}
					else
					{
						game_->GoToWorld(next_path);
					}
				}

				if (ImGui::Button("Back to this level", ImVec2(VICTORY_MODAL_WIDTH, VICTORY_BUTTON_HEIGHT)))
				{
					world_->Reset();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Back to main menu", ImVec2(VICTORY_MODAL_WIDTH, VICTORY_BUTTON_HEIGHT)))
				{
					editor_thread_->SetWorld(nullptr);
					game_->GoToMainMenu();
				}

				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::PushOverrideID(MENU_MODAL_ID);
			ImGui::SetNextWindowSize(ImVec2(MENU_MODAL_WIDTH, 0), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), ImGuiCond_Appearing, ImVec2(0.5, 0.5));
			if (ImGui::BeginPopupModal(MENU_MODAL_NAME, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 8.0f));

				if (ImGui::Button("Resume", ImVec2(MENU_MODAL_WIDTH, MENU_BUTTON_HEIGHT)))
				{
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Back to main menu", ImVec2(MENU_MODAL_WIDTH, MENU_BUTTON_HEIGHT)))
				{
					ImGui::PushOverrideID(EXIT_MODAL_ID);
					ImGui::OpenPopup(EXIT_MODAL_NAME);
					ImGui::PopID();
				}

				ImGui::PopStyleVar();

				ImGui::PushOverrideID(EXIT_MODAL_ID);
				ImGui::SetNextWindowSize(ImVec2(EXIT_MODAL_WIDTH, 0), ImGuiCond_Appearing);
				if (ImGui::BeginPopupModal(EXIT_MODAL_NAME, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::TextWrapped("You will lose your progress in this level.");

					ImGui::SetCursorPosX(EXIT_MODAL_WIDTH - 100 * 2 - 8);
					if (ImGui::Button("No", ImVec2(100, 0)))
					{
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();
					ImGui::SetCursorPosX(EXIT_MODAL_WIDTH - 100);
					if (ImGui::Button("Yes", ImVec2(100, 0)))
					{
						editor_thread_->SetWorld(nullptr);
						game_->GoToMainMenu();
					}

					ImGui::EndPopup();
				}
				ImGui::PopID();

				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
	}
}