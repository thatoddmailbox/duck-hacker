#ifndef _RENDER_MATERIAL_FACTORY_HPP
#define _RENDER_MATERIAL_FACTORY_HPP

#include <map>
#include <string>

#include "render/material.hpp"

namespace duckhacker
{
	namespace render
	{
		class MaterialFactory
		{
		public:
			static std::map<std::string, Material> MTL(const char * path);

		private:
			MaterialFactory() {}
		};
	}
}

#endif