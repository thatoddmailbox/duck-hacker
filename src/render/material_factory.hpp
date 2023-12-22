#ifndef _RENDER_MATERIAL_FACTORY_HPP
#define _RENDER_MATERIAL_FACTORY_HPP

#include <map>
#include <string>

#include "content/manager.hpp"

#include "render/material.hpp"

namespace duckhacker
{
	namespace render
	{
		class MaterialFactory
		{
		public:
			static std::map<std::string, Material> MTL(content::Manager * content_manager, const char * path);

		private:
			MaterialFactory() {}
		};
	}
}

#endif