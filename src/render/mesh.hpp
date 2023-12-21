#ifndef _RENDER_MESH_HPP
#define _RENDER_MESH_HPP

#include <cstddef>
#include <cstdlib>

#include <tuple>
#include <vector>

#include "glad/glad.h"

#include "glm/glm.hpp"

#include "material.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace duckhacker
{
	namespace render
	{
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

			void SetMaterial(const Material& material);
			void SetMaterials(const std::vector<std::tuple<size_t, Material>>& materials);

			void SetTexture(Texture * texture);
			void Draw(glm::mat4 * projection, glm::mat4 * view, glm::mat4 * model, glm::mat3 * normal, glm::vec3 * camera_position, Light * light);

		private:
			std::vector<std::tuple<size_t, Material>> materials_;

			Shader * shader_;

			float * vertices_;
			size_t vertices_size_;
			size_t vertices_count_;

			Texture * texture_;

			GLuint buffer_id_;
			GLuint vertex_array_id_;
		};
	}
}

#endif