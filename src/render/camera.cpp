#include "render/camera.hpp"

namespace duckhacker
{
	namespace render
	{
		Camera::Camera(size_t viewport_width, size_t viewport_height)
		{
			projection_ = glm::perspective(glm::radians(45.0f), ((float) viewport_width) / ((float) viewport_height), 0.1f, 100.0f);
			UpdateMatrices_();
		}

		const glm::vec3& Camera::GetPosition()
		{
			return position_;
		}

		void Camera::SetPosition(const glm::vec3& position)
		{
			position_ = position;
			UpdateMatrices_();
		}

		void Camera::SetRotation(const glm::vec3& rotation)
		{
			rotation_ = rotation;
			UpdateMatrices_();
		}

		void Camera::UpdateMatrices_()
		{
			view_ = glm::mat4(1.0f);
			view_ = glm::rotate(view_, glm::radians(-rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
			view_ = glm::rotate(view_, glm::radians(-rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
			view_ = glm::rotate(view_, glm::radians(-rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
			view_ = glm::translate(view_, position_ * glm::vec3(-1.0f));
		}

		glm::mat4 * Camera::GetProjection()
		{
			return &projection_;
		}

		glm::mat4 * Camera::GetView()
		{
			return &view_;
		}
	}
}