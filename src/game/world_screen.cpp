#include "game/world_screen.hpp"

namespace duckhacker
{
	namespace game
	{
		WorldScreen::WorldScreen(content::Manager * content_manager, game::editor::EditorThread * editor_thread, world::World * world)
		{
			editor_thread_ = editor_thread;

			render::Shader * bla = content_manager->Shader("shaders/gray");
			bot_mesh_ = content_manager->Mesh("models/duckbot.obj", bla);

			world_ = world;

			main_camera_.SetPosition(glm::vec3(-4, 2, 8));
			main_camera_.SetRotation(glm::vec3(0, -25, 0));
		}

		WorldScreen::~WorldScreen()
		{

		}

		void WorldScreen::Update(double dt, input::Manager * input_manager)
		{
			world_->Update(dt);
		}

		void WorldScreen::Draw(content::Manager * content_manager)
		{
			render::Light light;
			light.Position = glm::vec3(-4, 2, 8);
			light.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			light.Ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
			light.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			light.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			glm::vec3 camera_position = main_camera_.GetPosition();

			for (world::Bot * bot : world_->bots)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), bot->GetDisplayCoords());
				model = glm::rotate(model, glm::radians(-bot->GetDisplayRotation()), glm::vec3(0, 1, 0));
				glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));

				bot_mesh_->Draw(main_camera_.GetProjection(), main_camera_.GetView(), &model, &normal, &camera_position, &light);
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