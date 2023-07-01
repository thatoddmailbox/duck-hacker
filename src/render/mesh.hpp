#ifndef _RENDER_MESH_HPP
#define _RENDER_MESH_HPP

#include <cstddef>
#include <cstdlib>

#include "glad/glad.h"

#include "glm/glm.hpp"

#include "shader.hpp"

namespace duckhacker
{
	namespace render
	{
		struct MaterialProperties
		{
			glm::vec4 Ambient;
			glm::vec4 Diffuse;
			glm::vec4 Specular;

			float Shininess;
		};

		struct Light
		{
			glm::vec3 Position;
			glm::vec4 Color;

			glm::vec4 Ambient;
			glm::vec4 Diffuse;
			glm::vec4 Specular;
		};

		class Mesh
		{
		public:
			Mesh(Shader * shader, float * vertices, size_t vertices_size, size_t vertices_count);
			~Mesh();

			void Draw(glm::mat4 * projection, glm::mat4 * view, glm::mat4 * model, glm::mat3 * normal, glm::vec3 * camera_position, Light * light);

			MaterialProperties material_properties;

		private:
			Shader * shader_;

			float * vertices_;
			size_t vertices_size_;
			size_t vertices_count_;

			// Texture * texture_;

			GLuint buffer_id_;
			GLuint vertex_array_id_;
		};
	}
}

#endif