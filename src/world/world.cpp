#include "world/world.hpp"

#include "render/mesh_factory.hpp"

#include "pugixml.hpp"

namespace duckhacker
{
	namespace world
	{
		World::World(content::Manager * content_manager, std::string file_path)
		{
			// TODO: load via physfs
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(file_path.c_str());
			if (!result)
			{
				std::cout << "Failed to load world." << std::endl;
				return;
			}

			pugi::xml_node world_node = doc.child("world");

			pugi::xml_node field_node = world_node.child("field");
			int field_width = field_node.attribute("width").as_int();
			int field_height = field_node.attribute("height").as_int();

			center_point_ = glm::vec3(field_width / 2, 0, field_height / 2);

			render::Object floor = render::Object();
			floor.SetMesh(render::MeshFactory::Box(content_manager->Shader("shaders/basic"), field_width, 1, field_height));
			floor.SetPosition(glm::vec3(field_width / 2, -1, field_height / 2));
			objects.push_back(floor);

			pugi::xml_node bots_node = world_node.child("bots");

			for (pugi::xml_node bot_node : bots_node.children("bot"))
			{
				int id = bot_node.attribute("id").as_int();
				int x = bot_node.attribute("x").as_int();
				int y = bot_node.attribute("y").as_int();
				int z = bot_node.attribute("z").as_int();
				int rotation = bot_node.attribute("rotation").as_int();

				Bot * bot = new Bot(content_manager, id, x, y, z, rotation);
				bots.push_back(bot);
			}

			pugi::xml_node npcs_node = world_node.child("npcs");

			for (pugi::xml_node npc_node : npcs_node.children("npc"))
			{
				int id = npc_node.attribute("id").as_int();
				int x = npc_node.attribute("x").as_int();
				int y = npc_node.attribute("y").as_int();
				int z = npc_node.attribute("z").as_int();
				int rotation = npc_node.attribute("rotation").as_int();

				NPC * npc = new NPC(content_manager, id, x, y, z, rotation);
				npcs.push_back(npc);
			}

			pugi::xml_node objects_node = world_node.child("objects");

			for (pugi::xml_node object_node : objects_node.children("object"))
			{
				pugi::xml_node mesh_node = object_node.child("mesh");
				std::string mesh_type = mesh_node.attribute("type").as_string();

				render::Shader * shader = content_manager->Shader("shaders/basic");
				render::Mesh * mesh = nullptr;

				if (mesh_type == "cube")
				{
					int width = mesh_node.attribute("w").as_int();
					int height = mesh_node.attribute("h").as_int();
					int depth = mesh_node.attribute("d").as_int();

					mesh = render::MeshFactory::Box(shader, width, height, depth);
				}

				int x = object_node.attribute("x").as_int();
				int y = object_node.attribute("y").as_int();
				int z = object_node.attribute("z").as_int();
				int rotation = object_node.attribute("rotation").as_int();

				std::vector<render::Material> materials;
				for (pugi::xml_node material_node : mesh_node.children("material"))
				{
					render::Material m;

					pugi::xml_node ambient_node = material_node.child("ambient");
					if (!ambient_node.empty())
					{
						m.SetAmbient(glm::vec4(ambient_node.attribute("r").as_float(), ambient_node.attribute("g").as_float(), ambient_node.attribute("b").as_float(), 1.0f));
					}

					pugi::xml_node diffuse_node = material_node.child("diffuse");
					if (!diffuse_node.empty())
					{
						m.SetDiffuse(glm::vec4(diffuse_node.attribute("r").as_float(), diffuse_node.attribute("g").as_float(), diffuse_node.attribute("b").as_float(), 1.0f));
					}

					pugi::xml_node specular_node = material_node.child("specular");
					if (!specular_node.empty())
					{
						m.SetSpecular(glm::vec4(specular_node.attribute("r").as_float(), specular_node.attribute("g").as_float(), specular_node.attribute("b").as_float(), 1.0f));
					}

					pugi::xml_node shininess_node = material_node.child("shininess");
					if (!shininess_node.empty())
					{
						m.SetShininess(shininess_node.attribute("value").as_float());
					}

					pugi::xml_node texture_node = material_node.child("texture");
					if (!texture_node.empty())
					{
						m.SetTexture(content_manager->Texture(texture_node.attribute("path").as_string()));
					}

					materials.push_back(m);
				}

				if (materials.size() > 0)
				{
					if (mesh_type == "cube")
					{
						if (materials.size() != 1)
						{
							std::cout << "Cube mesh can only have one material." << std::endl;
						}

						mesh->SetMaterial(materials[0]);
					}
				}

				render::Object object = render::Object();
				object.SetMesh(mesh);
				object.SetPosition(glm::vec3(x, y, z));
				object.SetRotation(glm::vec3(0, -rotation, 0));
				objects.push_back(object);
			}
		}

		World::~World()
		{
			for (Bot * bot : bots)
			{
				delete bot;
			}
		}

		const glm::vec3& World::GetCenterPoint()
		{
			return center_point_;
		}

		const State& World::GetState()
		{
			return state_;
		}

		void World::Run()
		{
			state_ = State::RUNNING;

			for (Bot * bot : bots)
			{
				bot->Execute();
			}
		}

		void World::Stop()
		{
			for (Bot * bot : bots)
			{
				bot->RequestStop();
			}

			for (Bot * bot : bots)
			{
				bot->WaitForStop();
			}

			// TOOD: reset the world or something
			state_ = State::READY;
		}

		void World::Update(float dt)
		{
			if (state_ != State::RUNNING)
			{
				return;
			}

			for (Bot * bot : bots)
			{
				bot->Update(dt);
			}
		}
	}
}