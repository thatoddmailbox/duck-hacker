#include "render/material.hpp"

namespace duckhacker
{
	namespace render
	{
		Material::Material()
		{
			ambient_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			diffuse_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			specular_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			shininess_ = 0.0f;
		}

		glm::vec4& Material::GetAmbient()
		{
			return ambient_;
		}

		glm::vec4& Material::GetDiffuse()
		{
			return diffuse_;
		}

		glm::vec4& Material::GetSpecular()
		{
			return specular_;
		}

		float Material::GetShininess()
		{
			return shininess_;
		}

		void Material::SetAmbient(glm::vec4 value)
		{
			ambient_ = value;
		}

		void Material::SetDiffuse(glm::vec4 value)
		{
			diffuse_ = value;
		}

		void Material::SetSpecular(glm::vec4 value)
		{
			specular_ = value;
		}

		void Material::SetShininess(float shininess)
		{
			shininess_ = shininess;
		}
	}
}