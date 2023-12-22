#ifndef _RENDER_MESH_FACTORY_HPP
#define _RENDER_MESH_FACTORY_HPP

#include <cstddef>

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

#include "physfs.h"

#include "content/manager.hpp"

#include "render/mesh.hpp"

namespace duckhacker
{
	namespace render
	{
		/*
		* width is along x
		* length is along y
		* height is along z
		*/
		class MeshFactory
		{
		public:
			static Mesh * Box(Shader * shader, float width, float length, float depth);
			static Mesh * OBJ(content::Manager * content_manager, Shader * shader, const char * path);

		private:
			MeshFactory() {}
		};
	}
}

#endif