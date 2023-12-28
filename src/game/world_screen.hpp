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
	class Game;

	namespace game
	{
		class WorldScreen : public Screen
		{
		public:
			WorldScreen(Game * game, content::Manager * content_manager, game::editor::EditorThread * editor_thread, world::World * world);
			~WorldScreen();

			void SetWorld(world::World * world);

			void Update(double dt, input::Manager * input_manager) override;
			void Draw(content::Manager * content_manager) override;

		private:
			Game * game_;

			game::editor::EditorThread * editor_thread_;
			world::World * world_;

			bool no_ui_;
			bool opened_mission_;

			std::vector<render::Light> lights_;

			float yaw_;
			float pitch_;
			float radius_;
			float target_radius_;
			glm::vec3 focus_point_;

			render::Camera main_camera_ = render::Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
		};
	}
}

#endif