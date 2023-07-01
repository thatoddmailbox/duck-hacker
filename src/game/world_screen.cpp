#include "game/world_screen.hpp"

namespace duckhacker
{
	namespace game
	{
		WorldScreen::WorldScreen(content::Manager * content_manager)
		{
			render::Shader * bla = content_manager->Shader("shaders/gray");
			bot_mesh_ = render::MeshFactory::Box(bla, 1, 1, 1);

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
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));

			glm::vec3 camera_position = main_camera_.GetPosition();

			bot_mesh_->Draw(main_camera_.GetProjection(), main_camera_.GetView(), &model, &normal, &camera_position);
		}
	}
}