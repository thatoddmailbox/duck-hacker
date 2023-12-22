#ifndef _RENDER_MATERIAL_HPP
#define _RENDER_MATERIAL_HPP

#include <ostream>

#include "glm/glm.hpp"

namespace duckhacker
{
	namespace render
	{
		class Material
		{
		public:
			Material();

			glm::vec4& GetAmbient();
			glm::vec4& GetDiffuse();
			glm::vec4& GetSpecular();
			float GetShininess();

			void SetAmbient(glm::vec4 value);
			void SetDiffuse(glm::vec4 value);
			void SetSpecular(glm::vec4 value);
			void SetShininess(float shininess);

			friend std::ostream& operator<<(std::ostream& out, const Material& obj);

		private:
			glm::vec4 ambient_;
			glm::vec4 diffuse_;
			glm::vec4 specular_;
			float shininess_;
		};
	}
}

#endif