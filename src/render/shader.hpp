#ifndef _RENDER_SHADER_HPP
#define _RENDER_SHADER_HPP

#include <iostream>
#include <map>

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.hpp"

namespace duckhacker
{
	namespace render
	{
		class Shader
		{
		public:
			Shader(const char * vertex_source, const char * fragment_source);
			~Shader();

			void SetUniformFloat(const std::string& name, float value);
			void SetUniformVector3(const std::string& name, glm::vec3 * value);
			void SetUniformVector4(const std::string& name, glm::vec4 * value);
			void SetUniformMatrix3x3(const std::string& name, glm::mat3 * value);
			void SetUniformMatrix4x4(const std::string& name, glm::mat4 * value);
			void SetUniformTexture(const std::string& name, Texture * value);

			void Activate();

		private:
			Texture * bound_texture_;

			GLuint program_;
			bool created_;
		};

		enum ShaderLocation
		{
			POSITION = 0,
			NORMAL = 1,
			TEXTURE_UV = 2
		};
	}
}

#endif