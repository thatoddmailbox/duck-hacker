#include "content/manager.hpp"

#include "external/physfsrwops.h"

#include "defs.hpp"
#include "render/mesh_factory.hpp"

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

			PHYSFS_file * text_font_file = PHYSFS_openRead("fonts/Lato-Regular.ttf");
			int64_t text_font_file_length = PHYSFS_fileLength(text_font_file);
			char * text_font_file_data = (char *) malloc(text_font_file_length + 1);
			PHYSFS_readBytes(text_font_file, text_font_file_data, text_font_file_length);
			PHYSFS_close(text_font_file);

			PHYSFS_file * icon_font_file = PHYSFS_openRead("fonts/fa-solid-900.ttf");
			int64_t icon_font_file_length = PHYSFS_fileLength(icon_font_file);
			char * icon_font_file_data = (char *) malloc(icon_font_file_length + 1);
			PHYSFS_readBytes(icon_font_file, icon_font_file_data, icon_font_file_length);
			PHYSFS_close(icon_font_file);

			static const ImWchar text_range[] =
			{
				0x0020, 0x007F,
				0,
			};
			static const ImWchar icon_range[] =
			{
				0xF04B, 0xF04D,
				0,
			};

			ImFontConfig config;
			config.FontDataOwnedByAtlas = false;

			io.Fonts->AddFontFromMemoryTTF(text_font_file_data, text_font_file_length, SizeOfFont(FontType::REGULAR), &config, text_range);
			config.MergeMode = true;
			io.Fonts->AddFontFromMemoryTTF(icon_font_file_data, icon_font_file_length, SizeOfFont(FontType::REGULAR), &config, icon_range);
			io.Fonts->Build();
			font_regular_ = io.Fonts->Fonts[0];

			config.MergeMode = false;
			io.Fonts->AddFontFromMemoryTTF(text_font_file_data, text_font_file_length, SizeOfFont(FontType::MEDIUM), &config, text_range);
			config.MergeMode = true;
			io.Fonts->AddFontFromMemoryTTF(icon_font_file_data, icon_font_file_length, SizeOfFont(FontType::MEDIUM), &config, icon_range);
			io.Fonts->Build();
			font_medium_ = io.Fonts->Fonts[1];

			config.MergeMode = false;
			io.Fonts->AddFontFromMemoryTTF(text_font_file_data, text_font_file_length, SizeOfFont(FontType::LARGE), &config, text_range);
			config.MergeMode = true;
			io.Fonts->AddFontFromMemoryTTF(icon_font_file_data, icon_font_file_length, SizeOfFont(FontType::LARGE), &config, icon_range);
			io.Fonts->Build();
			font_large_ = io.Fonts->Fonts[2];

			config.MergeMode = false;
			io.Fonts->AddFontFromMemoryTTF(text_font_file_data, text_font_file_length, SizeOfFont(FontType::TITLE), &config, text_range);
			config.MergeMode = true;
			io.Fonts->AddFontFromMemoryTTF(icon_font_file_data, icon_font_file_length, SizeOfFont(FontType::TITLE), &config, icon_range);
			io.Fonts->Build();
			font_title_ = io.Fonts->Fonts[3];

			free(text_font_file_data);
			free(icon_font_file_data);
		}

		ImFont * Manager::Font(const FontType type)
		{
			if (type == FontType::LARGE)
			{
				return font_large_;
			}
			else if (type == FontType::MEDIUM)
			{
				return font_medium_;
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

		render::Mesh * Manager::Mesh(const std::string& path, render::Shader * shader)
		{
			std::map<std::string, render::Mesh *>::iterator mesh_iter = meshes_.find(path);
			if (mesh_iter == meshes_.end())
			{
				meshes_[path] = render::MeshFactory::OBJ(this, shader, path.c_str());

				mesh_iter = meshes_.find(path);
			}
			return mesh_iter->second;
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