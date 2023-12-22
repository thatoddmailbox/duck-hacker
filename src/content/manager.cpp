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
			for (const std::pair<std::string, render::Texture *>& texture : textures_)
			{
				delete texture.second;
			}
		}

		void Manager::LoadFonts()
		{
			ImGuiIO& io = ImGui::GetIO(); (void)io;

			ImFontConfig config;
			config.FontDataOwnedByAtlas = false;
			PHYSFS_file * font_file = PHYSFS_openRead("fonts/Lato-Regular.ttf");
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

		render::Texture * Manager::Texture(const std::string& path)
		{
			std::map<std::string, render::Texture *>::iterator texture_iter = textures_.find(path);
			if (texture_iter == textures_.end())
			{
				textures_[path] = new render::Texture(path.c_str());
				texture_iter = textures_.find(path);
			}
			return texture_iter->second;
		}
	};
};