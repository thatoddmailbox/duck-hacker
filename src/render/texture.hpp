#ifndef _RENDER_TEXTURE_HPP
#define _RENDER_TEXTURE_HPP

#include <cstdint>
#include <cstdlib>

#include "glad/glad.h"

namespace duckhacker
{
	namespace content
	{
		class Manager;
	}

	namespace render
	{
		class Shader;

		class Texture
		{
		public:
			Texture(content::Manager * content_manager, const char * path);
			~Texture();

			GLuint GetTextureID() const;

		private:
			friend class Shader;

			uint64_t width_;
			uint64_t height_;

			GLuint texture_;
		};
	}
}

#endif