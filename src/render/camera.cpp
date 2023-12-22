#include "render/camera.hpp"

#include "glm/gtc/matrix_access.hpp"

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

		void Camera::LookAt(glm::vec3 target, glm::vec3 up)
		{
			glm::mat4 look_at_matrix = glm::lookAt(position_, target, up);

			// based on https://github.com/mrdoob/three.js/blob/ab1bac16e8d82c1d5c1dacb22d552a8fdced3add/src/math/Euler.js#L136

			float m11 = glm::row(look_at_matrix, 0)[0];
			float m12 = glm::row(look_at_matrix, 0)[1];
			float m13 = glm::row(look_at_matrix, 0)[2];
			float m22 = glm::row(look_at_matrix, 1)[1];
			float m23 = glm::row(look_at_matrix, 1)[2];
			float m32 = glm::row(look_at_matrix, 2)[1];
			float m33 = glm::row(look_at_matrix, 2)[2];

			float clampedm13 = (m13 < -1 ? -1 : (m13 > 1 ? 1 : m13));

			rotation_.y = glm::degrees(-asinf(clampedm13));

			if (abs(m13) < 0.9999999)
			{
				rotation_.x = glm::degrees(-atan2f(-m23, m33));
				rotation_.z = glm::degrees(atan2f(-m12, m11));
			}
			else
			{
				rotation_.x = glm::degrees(-atan2f(m32, m22));
				rotation_.z = 0;
			}

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