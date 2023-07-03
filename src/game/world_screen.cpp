#include "game/world_screen.hpp"

namespace duckhacker
{
	namespace game
	{
		WorldScreen::WorldScreen(content::Manager * content_manager, game::editor::EditorThread * editor_thread, world::World * world)
		{
			editor_thread_ = editor_thread;

			render::Shader * bla = content_manager->Shader("shaders/gray");
			bot_mesh_ = render::MeshFactory::Box(bla, 1, 1, 1);
			bot_mesh_->material_properties.Ambient = glm::vec4(0.0215f, 0.1745f, 0.0215f, 1.0f);
			bot_mesh_->material_properties.Diffuse = glm::vec4(0.07568f, 0.61424f, 0.07568f, 1.0f);
			bot_mesh_->material_properties.Specular = glm::vec4(0.633f, 0.727811, 0.633f, 1.0f);
			bot_mesh_->material_properties.Shininess = 0.6f;
			bot_mesh_->material_properties.Ambient = glm::vec4(0.19225, 0.19225, 0.19225, 1.0f);
			bot_mesh_->material_properties.Diffuse = glm::vec4(0.50754, 0.50754, 0.50754, 1.0f);
			bot_mesh_->material_properties.Specular = glm::vec4(0.508273, 0.508273, 0.508273, 1.0f);
			bot_mesh_->material_properties.Shininess = 0.4f;

			world_ = world;

			main_camera_.SetPosition(glm::vec3(-4, 2, 8));
			main_camera_.SetRotation(glm::vec3(0, -25, 0));
		}

		WorldScreen::~WorldScreen()
		{

		}

		void WorldScreen::Update(double dt)
		{

		}

		void WorldScreen::Draw(content::Manager * content_manager)
		{
			render::Light light;
			light.Position = glm::vec3(-4, 2, 8);
			light.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			light.Ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			light.Diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			light.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			glm::vec3 camera_position = main_camera_.GetPosition();

			for (world::Bot * bot : world_->bots)
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(bot->x, bot->y, bot->z));
				glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));

				bot_mesh_->Draw(main_camera_.GetProjection(), main_camera_.GetView(), &model, &normal, &camera_position, &light);
			}


			for (world::Bot * bot : world_->bots)
			{
				ImGui::Begin("Bot");

				ImGui::Text("beep bloop");
				ImGui::Text("position %d %d %d\n", bot->x, bot->y, bot->z);
				if (ImGui::Button("Edit code"))
				{
					editor_thread_->OpenEditor(bot->id, bot->code);
				}

				ImGui::End();
			}

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
						if (code.find(bot->id) == code.end())
						{
							// no code change to this bot
							continue;
						}

						bot->code = code[bot->id];
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