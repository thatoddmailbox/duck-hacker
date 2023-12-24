#include "game/world_screen.hpp"

#include "render/object.hpp"

namespace duckhacker
{
	namespace game
	{
		WorldScreen::WorldScreen(content::Manager * content_manager, game::editor::EditorThread * editor_thread, world::World * world)
		{
			editor_thread_ = editor_thread;

			world_ = world;

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

			const glm::vec3& center_point = world_->GetCenterPoint();
			main_camera_.SetPosition(center_point + glm::vec3(
				radius_ * cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
				radius_ * sin(glm::radians(pitch_)),
				radius_ * sin(glm::radians(yaw_)) * cos(glm::radians(pitch_))
			));
			main_camera_.LookAt(center_point, glm::vec3(0, 1, 0));
		}

		static void ShowTooltip(glm::vec2 anchor_point, const char * text)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 text_size = ImGui::CalcTextSize(text);

			int window_width = text_size.x + style.WindowPadding.x * 2;
			int window_height = text_size.y + style.WindowPadding.y * 2;

			ImGui::SetNextWindowPos(ImVec2(anchor_point.x - (window_width / 2), anchor_point.y - (window_height / 2)), ImGuiCond_Always);

			ImGui::Begin("##Tooltip", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

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
			}

			for (world::NPC * npc : world_->npcs)
			{
				npc->object.Draw(main_camera_.GetProjection(), main_camera_.GetView(), &camera_position, lights_);
			}

			for (render::Object& object : world_->objects)
			{
				object.Draw(main_camera_.GetProjection(), main_camera_.GetView(), &camera_position, lights_);
			}

			glm::vec2 bla = world_->bots[0]->object.ObjectPositionToScreenPosition(glm::vec3(0, 0, 0), main_camera_.GetProjection(), main_camera_.GetView());
			bla *= glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
			ShowTooltip(bla, "test");

			ImGui::SetNextWindowPos(ImVec2(50, 125), ImGuiCond_Appearing);
			if (ImGui::Begin("World"))
			{
				for (world::Bot * bot : world_->bots)
				{
					if (ImGui::Selectable(bot->GetName().c_str()))
					{
						editor_thread_->OpenEditor(bot->GetID(), bot->code);
					}
					// ImGui::Text("position %d %d %d\n", bot->GetX(), bot->GetY(), bot->GetZ());
					// ImGui::Text("rotation %d\n", bot->GetRotation());
				}

				for (world::NPC * npc : world_->npcs)
				{
					ImGui::Selectable(npc->GetName().c_str());
					// ImGui::Text("position %d %d %d\n", npc->GetX(), npc->GetY(), npc->GetZ());
					// ImGui::Text("rotation %d\n", npc->GetRotation());
				}
			}

			ImGui::End();

			ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Appearing);
			ImGui::Begin("Control");

			if (world_->GetState() == world::State::READY)
			{
				if (ImGui::Button("Start"))
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

						bot->code = code[bot->GetID()];
					}

					// run all bots!
					world_->Run();
				}
			}
			else
			{
				if (ImGui::Button("Stop"))
				{
					world_->Stop();
				}
			}

			ImGui::End();

			if (ImGui::Begin("Scene settings"))
			{
				ImGui::SeparatorText("Camera");
				ImGui::SliderFloat("Radius##CameraRadius", &target_radius_, 1, 100);
				ImGui::SliderFloat("Yaw##CameraYaw", &yaw_, 0, 360);
				ImGui::SliderFloat("Pitch##CameraPitch", &pitch_, -89, 89);

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
			}

			ImGui::End();
		}
	}
}