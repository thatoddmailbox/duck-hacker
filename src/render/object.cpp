#include "render/object.hpp"

#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace duckhacker
{
	namespace render
	{
		Object::Object()
		{
			mesh_ = nullptr;
		}

		Mesh * Object::GetMesh()
		{
			return mesh_;
		}

		void Object::SetMesh(Mesh * mesh)
		{
			mesh_ = mesh;
		}

		const glm::vec3& Object::GetPosition()
		{
			return position_;
		}

		const glm::vec3& Object::GetRotation()
		{
			return rotation_;
		}

		const glm::vec3& Object::GetScale()
		{
			return scale_;
		}

		void Object::SetPosition(const glm::vec3& position)
		{
			position_ = position;
			location_dirty_ = true;
		}

		void Object::SetRotation(const glm::vec3& rotation)
		{
			rotation_ = rotation;
			location_dirty_ = true;
		}

		void Object::SetScale(const glm::vec3& scale)
		{
			scale_ = scale;
			location_dirty_ = true;
		}

		glm::vec2 Object::ObjectPositionToScreenPosition(const glm::vec3& object_position, glm::mat4 * projection, glm::mat4 * view)
		{
			glm::vec4 clip_position = *projection * *view * glm::translate(glm::mat4(1.0f), position_) * glm::vec4(object_position, 1.0f);
			glm::vec3 ndc_position = glm::vec3(clip_position) / clip_position.w;
			glm::vec2 screen_position = glm::vec2(ndc_position.x, ndc_position.y);
			screen_position.x = (screen_position.x + 1.0f) / 2.0f;
			screen_position.y = (-screen_position.y + 1.0f) / 2.0f;
			return screen_position;
		}

		void Object::UpdateMatrices_()
		{
			model_ = glm::mat4(1.0f);
			model_ = glm::translate(model_, position_);
			model_ = glm::rotate(model_, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model_ = glm::rotate(model_, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model_ = glm::rotate(model_, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model_ = glm::scale(model_, scale_);

			normal_ = glm::mat3(glm::transpose(glm::inverse(model_)));

			location_dirty_ = false;
		}

		void Object::Draw(glm::mat4 * projection, glm::mat4 * view, glm::vec3 * camera_position, const std::vector<Light>& lights)
		{
			if (location_dirty_)
			{
				UpdateMatrices_();
			}

			if (!mesh_)
			{
				return;
			}

			mesh_->Draw(projection, view, &model_, &normal_, camera_position, lights);
		}
	}
}