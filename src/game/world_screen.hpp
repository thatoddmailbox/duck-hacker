#ifndef _GAME_WORLD_SCREEN_HPP
#define _GAME_WORLD_SCREEN_HPP

#include <SDL.h>

#include "glm/glm.hpp"

#include "content/manager.hpp"
#include "game/screen.hpp"
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
			WorldScreen(content::Manager * content_manager);
			~WorldScreen();

			void Update(double dt) override;
			void Draw(content::Manager * content_manager) override;

		private:
			world::World * world_;

			// TODO: would be nice to not hardcode this
			render::Camera main_camera_ = render::Camera(800, 600);
			render::Mesh * bot_mesh_;
		};
	}
}

#endif