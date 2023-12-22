#include "render/shader.hpp"

namespace duckhacker
{
	namespace render
	{
		Shader::Shader(const char * vertex_source, const char * fragment_source)
		{
			created_ = false;
			bound_texture_ = nullptr;

			GLuint vertex_shader;
			GLuint fragment_shader;
			int success;

			vertex_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
			glCompileShader(vertex_shader);
			glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
				std::cout << "vertex shader compile failed\n" << infoLog << std::endl;
				return;
			}

			fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
			glCompileShader(fragment_shader);
			glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glDeleteShader(vertex_shader);

				char infoLog[512];
				glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
				std::cout << "fragment shader compile failed\n" << infoLog << std::endl;
				return;
			}

			program_ = glCreateProgram();
			glAttachShader(program_, vertex_shader);
			glAttachShader(program_, fragment_shader);
			glLinkProgram(program_);

			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);

			created_ = true;
		}

		Shader::~Shader()
		{
			if (created_)
			{
				glDeleteProgram(program_);
			}
		}

		void Shader::Activate()
		{
			glUseProgram(program_);
		}

		void Shader::SetUniformFloat(const std::string& name, float value)
		{
			GLuint uniform_id = glGetUniformLocation(program_, name.c_str());
			glUniform1fv(uniform_id, 1, &value);
		}

		void Shader::SetUniformVector3(const std::string& name, const glm::vec3 * value)
		{
			GLuint uniform_id = glGetUniformLocation(program_, name.c_str());
			glUniform3fv(uniform_id, 1, glm::value_ptr(*value));
		}

		void Shader::SetUniformVector4(const std::string& name, const glm::vec4 * value)
		{
			GLuint uniform_id = glGetUniformLocation(program_, name.c_str());
			glUniform4fv(uniform_id, 1, glm::value_ptr(*value));
		}

		void Shader::SetUniformMatrix3x3(const std::string& name, const glm::mat3 * value)
		{
			GLuint uniform_id = glGetUniformLocation(program_, name.c_str());
			glUniformMatrix3fv(uniform_id, 1, GL_FALSE, glm::value_ptr(*value));
		}

		void Shader::SetUniformMatrix4x4(const std::string& name, const glm::mat4 * value)
		{
			GLuint uniform_id = glGetUniformLocation(program_, name.c_str());
			glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(*value));
		}

		void Shader::SetUniformTexture(const std::string& name, Texture * value)
		{
			bound_texture_ = value;

			// HACK: we assume one texture per shader and so just use unit 0
			GLuint uniform_id = glGetUniformLocation(program_, name.c_str());
			glUniform1i(uniform_id, 0);

			if (bound_texture_)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, bound_texture_->texture_);
			}
		}
	}
}