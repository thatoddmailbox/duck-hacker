#ifndef _GAME_WORLD_SCREEN_HPP
#define _GAME_WORLD_SCREEN_HPP

#include <SDL.h>

#include "content/manager.hpp"
#include "game/screen.hpp"

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
		};
	}
}

#endif