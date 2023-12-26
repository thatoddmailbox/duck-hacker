#ifndef _GAME_WORLD_SCREEN_HPP
#define _GAME_WORLD_SCREEN_HPP

#include <SDL.h>

#include "glm/glm.hpp"

#include "defs.hpp"

#include "content/manager.hpp"
#include "game/screen.hpp"
#include "game/editor/editor_thread.hpp"
#include "input/manager.hpp"
#include "render/camera.hpp"
#include "render/mesh.hpp"
#include "render/mesh_factory.hpp"
#include "world/world.hpp"

namespace duckhacker
{
	namespace game
	{
		class WorldScreen : public Screen
		{
		public:
			WorldScreen(content::Manager * content_manager, game::editor::EditorThread * editor_thread, world::World * world);
			~WorldScreen();

			void Update(double dt, input::Manager * input_manager) override;
			void Draw(content::Manager * content_manager) override;

		private:
			game::editor::EditorThread * editor_thread_;
			world::World * world_;

			std::vector<render::Light> lights_;

			float yaw_ = 240.0f;
			float pitch_ = 22.0f;
			float radius_ = 8.0f;
			float target_radius_ = 8.0f;
			glm::vec3 focus_point_ = glm::vec3(0.0f, 0.0f, 0.0f);

			render::Camera main_camera_ = render::Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
		};
	}
}

#endif