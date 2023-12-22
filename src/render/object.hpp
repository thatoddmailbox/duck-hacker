#ifndef _RENDER_OBJECT_HPP
#define _RENDER_OBJECT_HPP

#include "glad/glad.h"

#include "glm/glm.hpp"

#include "mesh.hpp"

namespace duckhacker
{
	namespace render
	{
		class Object
		{
		public:
			Object();

			Mesh * GetMesh();
			void SetMesh(Mesh * mesh);

			const glm::vec3& GetPosition();
			const glm::vec3& GetRotation();
			const glm::vec3& GetScale();
			void SetPosition(const glm::vec3& position);
			void SetRotation(const glm::vec3& rotation);
			void SetScale(const glm::vec3& scale);

			void Draw(glm::mat4 * projection, glm::mat4 * view, glm::vec3 * camera_position, const std::vector<Light>& lights);

		private:
			void UpdateMatrices_();

			glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
			bool location_dirty_ = true;

			glm::mat4 model_;
			glm::mat3 normal_;

			Mesh * mesh_;
		};
	}
}

#endif