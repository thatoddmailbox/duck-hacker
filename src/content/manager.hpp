#ifndef _CONTENT_MANAGER_HPP
#define _CONTENT_MANAGER_HPP

#include <map>
#include <string>

#include <SDL.h>
#include <SDL_mixer.h>

#include "external/imgui/imgui.h"

#include "content/font_type.hpp"

#include "render/mesh.hpp"
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
						type == FontType::MEDIUM ? 20 :
						(type == FontType::TITLE ? 48 : 18));
			};

			char * File(const std::string& path, int64_t * length);

			ImFont * Font(const FontType type);
			render::Mesh * Mesh(const std::string& path, render::Shader * shader);
			Mix_Music * Music(const std::string& path);
			render::Shader * Shader(const std::string& path);
			Mix_Chunk * SoundEffect(const std::string& path);
			render::Texture * Texture(const std::string& path);

		protected:
			void LoadFonts();
			friend class duckhacker::Game;

		private:
			std::map<std::string, render::Mesh *> meshes_;
			std::map<std::string, Mix_Music *> musics_;
			std::map<std::string, render::Shader *> shaders_;
			std::map<std::string, Mix_Chunk *> sound_effects_;
			std::map<std::string, render::Texture *> textures_;

			ImFont * font_regular_;
			ImFont * font_medium_;
			ImFont * font_large_;
			ImFont * font_title_;
		};
	}
}

#endif