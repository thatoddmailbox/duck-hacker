#include "mesh.hpp"

#include <cstdio>
namespace duckhacker
{
	namespace render
	{
		Mesh::Mesh(Shader * shader, float * vertices, size_t vertices_size, size_t vertices_count) :
			shader_(shader), vertices_(vertices), vertices_size_(vertices_size), vertices_count_(vertices_count)
		{
			SetMaterial(Material());

			glGenBuffers(1, &buffer_id_);
			glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
			glBufferData(GL_ARRAY_BUFFER, vertices_size_, vertices, GL_STATIC_DRAW);

			// assume xyzuv layout
			glGenVertexArrays(1, &vertex_array_id_);
			glBindVertexArray(vertex_array_id_);

			glVertexAttribPointer(
				ShaderLocation::POSITION,
				3,
				GL_FLOAT,
				GL_FALSE,
				8 * sizeof(float),
				(GLvoid *) 0
			);
			glEnableVertexAttribArray(ShaderLocation::POSITION);

			glVertexAttribPointer(
				ShaderLocation::NORMAL,
				3,
				GL_FLOAT,
				GL_FALSE,
				8 * sizeof(float),
				(GLvoid *) (3 * sizeof(float))
			);
			glEnableVertexAttribArray(ShaderLocation::NORMAL);

			glVertexAttribPointer(
				ShaderLocation::TEXTURE_UV,
				2,
				GL_FLOAT,
				GL_FALSE,
				8 * sizeof(float),
				(GLvoid *) (6 * sizeof(float))
			);
			glEnableVertexAttribArray(ShaderLocation::TEXTURE_UV);
		}

		Mesh::~Mesh()
		{
			free(vertices_);
			glDeleteBuffers(1, &buffer_id_);
		}

		void Mesh::SetMaterial(const Material& material)
		{
			materials_.clear();
			materials_.push_back(std::make_tuple(0, material));
		}

		void Mesh::SetMaterials(const std::vector<std::tuple<size_t, Material>>& materials)
		{
			materials_ = materials;
		}

		void Mesh::Draw(glm::mat4 * projection, glm::mat4 * view, glm::mat4 * model, glm::mat3 * normal, glm::vec3 * camera_position, const std::vector<Light>& lights)
		{
			shader_->Activate();
			shader_->SetUniformMatrix4x4("projection", projection);
			shader_->SetUniformMatrix4x4("view", view);
			shader_->SetUniformMatrix4x4("model", model);
			shader_->SetUniformMatrix3x3("normal_matrix", normal);

			shader_->SetUniformVector3("camera_pos", camera_position);

			shader_->SetUniformVector3("light.position", &lights[0].Position);
			shader_->SetUniformVector4("light.color", &lights[0].Color);
			shader_->SetUniformVector4("light.ambient", &lights[0].Ambient);
			shader_->SetUniformVector4("light.diffuse", &lights[0].Diffuse);
			shader_->SetUniformVector4("light.specular", &lights[0].Specular);

			glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
			glBindVertexArray(vertex_array_id_);

			size_t remaining_vertices = vertices_count_;
			size_t offset = 0;
			for (auto& material_pair : materials_)
			{
				size_t count = std::get<0>(material_pair);
				if (count == 0)
				{
					count = remaining_vertices;
				}

				Material& m = std::get<1>(material_pair);
				shader_->SetUniformVector4("material.ambient", &m.GetAmbient());
				shader_->SetUniformVector4("material.diffuse", &m.GetDiffuse());
				if (m.GetTexture())
				{
					shader_->SetUniformTexture("material.diffuse_map", m.GetTexture());
					shader_->SetUniformFloat("material.diffuse_map_enabled", 1.0f);
				}
				else
				{
					shader_->SetUniformFloat("material.diffuse_map_enabled", 0.0f);
				}
				shader_->SetUniformVector4("material.specular", &m.GetSpecular());
				shader_->SetUniformFloat("material.shininess", m.GetShininess());

				glDrawArrays(GL_TRIANGLES, offset, count);

				remaining_vertices -= count;
				offset += count;
			}
		}
	}
}