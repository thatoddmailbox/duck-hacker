#ifndef _RENDER_CAMERA_HPP
#define _RENDER_CAMERA_HPP

#include <cstddef>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace duckhacker
{
	namespace render
	{
		class Camera
		{
		public:
			Camera(size_t viewport_width, size_t viewport_height);

			const glm::vec3& GetPosition();

			void SetPosition(const glm::vec3& position);
			void SetRotation(const glm::vec3& rotation);

			void LookAt(glm::vec3 target, glm::vec3 up);

			glm::mat4 * GetProjection();
			glm::mat4 * GetView();

		private:
			void UpdateMatrices_();

			glm::mat4 projection_;
			glm::mat4 view_;

			glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);
		};
	}
}

#endif