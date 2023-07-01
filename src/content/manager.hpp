#ifndef _CONTENT_MANAGER_HPP
#define _CONTENT_MANAGER_HPP

#include <map>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "external/imgui/imgui.h"

#include "content/font_type.hpp"

#include "render/shader.hpp"

namespace duckhacker
{
	class Game;

	namespace content
	{
		class Manager
		{
		public:
			Manager();
			~Manager();

			Manager(Manager& other) = delete;
			Manager(Manager&& other) = delete;

			constexpr static int SizeOfFont(const FontType type)
			{
				return (type == FontType::LARGE ? 24 :
						(type == FontType::TITLE ? 36 : 18));
			};

			ImFont * Font(const FontType type);
			SDL_Surface * Image(const std::string& path);
			render::Shader * Shader(const std::string& path);

		protected:
			void LoadFonts();
			friend class duckhacker::Game;

		private:
			std::map<std::string, render::Shader *> shaders_;
			std::map<std::string, SDL_Surface *> surfaces_;

			ImFont * font_regular_;
			ImFont * font_large_;
			ImFont * font_title_;
		};
	};
};

#endif