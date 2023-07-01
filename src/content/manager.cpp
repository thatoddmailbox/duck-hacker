#include "content/manager.hpp"

#include "external/physfsrwops.h"

namespace duckhacker
{
	namespace content
	{
		Manager::Manager()
		{

		}

		Manager::~Manager()
		{
			for (const std::pair<std::string, SDL_Surface *>& surface : surfaces_)
			{
				SDL_FreeSurface(surface.second);
			}
		}

		void Manager::LoadFonts()
		{
			ImGuiIO& io = ImGui::GetIO(); (void)io;

			ImFontConfig config;
			config.FontDataOwnedByAtlas = false;
			PHYSFS_file * font_file = PHYSFS_openRead("Lato-Regular.ttf");
			int64_t font_file_length = PHYSFS_fileLength(font_file);
			char * font_file_data = (char *) malloc(font_file_length + 1);
			PHYSFS_readBytes(font_file, font_file_data, font_file_length);
			PHYSFS_close(font_file);

			static const ImWchar text_range[] =
			{
				0x0020, 0x007F,
				0,
			};
			font_regular_ = io.Fonts->AddFontFromMemoryTTF(font_file_data, font_file_length, SizeOfFont(FontType::REGULAR), &config, text_range);
			font_large_ = io.Fonts->AddFontFromMemoryTTF(font_file_data, font_file_length, SizeOfFont(FontType::LARGE), &config, text_range);
			font_title_ = io.Fonts->AddFontFromMemoryTTF(font_file_data, font_file_length, SizeOfFont(FontType::TITLE), &config, text_range);
			io.Fonts->Build();
			free(font_file_data);
		}

		ImFont * Manager::Font(const FontType type)
		{
			if (type == FontType::LARGE)
			{
				return font_large_;
			}
			else if (type == FontType::TITLE)
			{
				return font_title_;
			}

			return font_regular_;
		}

		SDL_Surface * Manager::Image(const std::string& path)
		{
			std::map<std::string, SDL_Surface *>::iterator surface_iter = surfaces_.find(path);
			if (surface_iter == surfaces_.end())
			{
				surfaces_[path] = IMG_Load_RW(PHYSFSRWOPS_openRead(path.c_str()), 1);
				surface_iter = surfaces_.find(path);
			}
			return surface_iter->second;
		}

		static char * read_physfs_file(std::string path)
		{
			PHYSFS_File * file = PHYSFS_openRead(path.c_str());
			PHYSFS_sint64 file_length = PHYSFS_fileLength(file);
			char * file_data = (char *) malloc(file_length + 1);
			PHYSFS_readBytes(file, file_data, file_length);
			file_data[file_length] = '\0';
			PHYSFS_close(file);
			return file_data;
		}

		render::Shader * Manager::Shader(const std::string& path)
		{
			std::map<std::string, render::Shader *>::iterator shader_iter = shaders_.find(path);
			if (shader_iter == shaders_.end())
			{
				// TODO: don't copy the physfs files into memory
				char * vertex_source = read_physfs_file(path + ".vert.glsl");
				char * fragment_source = read_physfs_file(path + ".frag.glsl");

				shaders_[path] = new render::Shader(vertex_source, fragment_source);

				free(fragment_source);
				free(vertex_source);

				shader_iter = shaders_.find(path);
			}
			return shader_iter->second;
		}
	};
};