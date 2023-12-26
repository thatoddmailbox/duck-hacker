#include "render/texture.hpp"

#include <physfs.h>
#include <png.h>

#include "content/manager.hpp"

namespace duckhacker
{
	namespace render
	{
		Texture::Texture(content::Manager * content_manager, const char * path)
		{
			png_image image = {};
			image.opaque = nullptr;
			image.version = PNG_IMAGE_VERSION;

			// TODO: don't copy the file into memory
			int64_t file_length;
			char * file_data = content_manager->File(path, &file_length);

			png_image_begin_read_from_memory(&image, file_data, file_length);
			image.format = PNG_FORMAT_RGBA;
			width_ = image.width;
			height_ = image.height;

			unsigned char * texture_data = (unsigned char *) malloc(image.width * image.height * 4);
			png_image_finish_read(&image, nullptr, texture_data, image.width * 4, nullptr);

			glGenTextures(1, &texture_);
			glBindTexture(GL_TEXTURE_2D, texture_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

			if (GLAD_GL_EXT_texture_filter_anisotropic)
			{
				// TODO: should we complain more loudly if the user doesn't have this extension?
				float aniso = 0.0f;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
			}

			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			free(texture_data);
			free(file_data);
		}

		Texture::~Texture()
		{
			glDeleteTextures(1, &texture_);
		}
	}
}