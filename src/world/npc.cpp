#include "world/npc.hpp"
\
namespace duckhacker
{
	namespace world
	{
		NPC::NPC(content::Manager * content_manager, int id, int x, int y, int z, int rotation)
		{
			id_ = id;
			x_ = x;
			y_ = y;
			z_ = z;
			rotation_ = rotation;

			object.SetMesh(content_manager->Mesh("models/npc.obj", content_manager->Shader("shaders/basic")));
			object.SetPosition(glm::vec3(x_, y_, z_));
			object.SetRotation(glm::vec3(0, -rotation, 0));
		}

		NPC::~NPC()
		{

		}

		const int& NPC::GetID()
		{
			return id_;
		}

		const int& NPC::GetX()
		{
			return x_;
		}

		const int& NPC::GetY()
		{
			return y_;
		}

		const int& NPC::GetZ()
		{
			return z_;
		}

		const int& NPC::GetRotation()
		{
			return rotation_;
		}
	}
}