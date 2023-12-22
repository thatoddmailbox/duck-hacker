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

		static void output_vec4(std::ostream& out, const glm::vec4& v)
		{
			out << "(";
			out << v.r << ", ";
			out << v.g << ", ";
			out << v.b << ", ";
			out << v.a;
			out << ")";
		}

		std::ostream& operator<<(std::ostream& out, const Material& obj)
		{
			out << "Material(";
			out << "ambient=";
			output_vec4(out, obj.ambient_);
			out << ", diffuse=";
			output_vec4(out, obj.diffuse_);
			out << ", specular=";
			output_vec4(out, obj.specular_);
			out << ", shininess=";
			out << obj.shininess_;
			out << ")";
			return out;
		}
	}
}