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
		}

		WorldScreen::~WorldScreen()
		{

		}

		static float duck = 0;

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
			}

			if (target_radius_ != radius_)
			{
				radius_ += (target_radius_ - radius_) * 0.1f;

				if (abs(target_radius_ - radius_) < 0.001f)
				{
					radius_ = target_radius_;
				}
			}

			main_camera_.SetPosition(glm::vec3(
				radius_ * cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
				radius_ * sin(glm::radians(pitch_)),
				radius_ * sin(glm::radians(yaw_)) * cos(glm::radians(pitch_))
			));
			main_camera_.LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		}

		void WorldScreen::Draw(content::Manager * content_manager)
		{
			std::vector<render::Light> lights;
			render::Light light;
			light.Direction = glm::vec3(-1, -0.75, -1);
			light.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			light.Ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
			light.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			light.Specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			lights.push_back(light);

			glm::vec3 camera_position = main_camera_.GetPosition();

			for (world::Bot * bot : world_->bots)
			{
				bot->object.Draw(main_camera_.GetProjection(), main_camera_.GetView(), &camera_position, lights);
			}

			for (render::Object& object : world_->objects)
			{
				object.Draw(main_camera_.GetProjection(), main_camera_.GetView(), &camera_position, lights);
			}

			for (world::Bot * bot : world_->bots)
			{
				ImGui::SetNextWindowPos(ImVec2(50, 125), ImGuiCond_Appearing);
				ImGui::Begin("DuckBot");

				ImGui::Text("beep bloop");
				ImGui::Text("position %d %d %d\n", bot->GetX(), bot->GetY(), bot->GetZ());
				ImGui::Text("rotation %d\n", bot->GetRotation());
				if (ImGui::Button("Edit code"))
				{
					editor_thread_->OpenEditor(bot->GetID(), bot->code);
				}

				ImGui::End();
			}

			ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Appearing);
			ImGui::Begin("Control");

			if (world_->GetState() == world::WorldState::READY)
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
		}
	}
}