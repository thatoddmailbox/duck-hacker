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

			int64_t text_font_file_length;
			char * text_font_file_data = File("fonts/Lato-Regular.ttf", &text_font_file_length);

			int64_t icon_font_file_length;
			char * icon_font_file_data = File("fonts/fa-solid-900.ttf", &icon_font_file_length);

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

		char * Manager::File(const std::string& path, int64_t * length)
		{
			PHYSFS_File * file = PHYSFS_openRead(path.c_str());
			if (file == nullptr)
			{
				std::string physfs_msg = PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
				std::string error_text = "There was a problem loading file '" + path + "'.\n\nPhysFS Error: " + physfs_msg + "\n\nWe will probably crash now.";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error_text.c_str(), nullptr);
				return nullptr;
			}

			int64_t l = PHYSFS_fileLength(file);
			if (length != nullptr)
			{
				*length = l;
			}

			char * data = (char *) malloc(l + 1);
			PHYSFS_readBytes(file, data, l);
			data[l] = '\0';
			PHYSFS_close(file);
			return data;
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
				char * vertex_source = File(path + ".vert.glsl", nullptr);
				char * fragment_source = File(path + ".frag.glsl", nullptr);

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
				textures_[path] = new render::Texture(this, path.c_str());
				texture_iter = textures_.find(path);
			}
			return texture_iter->second;
		}
	};
};