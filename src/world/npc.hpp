#ifndef _WORLD_NPC_HPP
#define _WORLD_NPC_HPP

#include <string>

#include "content/manager.hpp"

#include "render/object.hpp"

namespace duckhacker
{
	namespace world
	{
		class NPC
		{
		public:
			NPC(content::Manager * content_manager, int id, int x, int y, int z, int rotation);
			~NPC();

			render::Object object;

			const int& GetID();
			const int& GetX();
			const int& GetY();
			const int& GetZ();
			const int& GetRotation();

		private:
			int id_;
			int x_, y_, z_;
			int rotation_;
		};
	}
}

#endif