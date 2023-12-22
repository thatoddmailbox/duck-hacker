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

			render::Object floor = render::Object();
			floor.SetMesh(render::MeshFactory::Box(content_manager->Shader("shaders/gray"), field_width, 1, field_height));
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
		}

		World::~World()
		{
			for (Bot * bot : bots)
			{
				delete bot;
			}
		}

		const WorldState& World::GetState()
		{
			return state_;
		}

		void World::Run()
		{
			state_ = WorldState::RUNNING;

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
			state_ = WorldState::READY;
		}

		void World::Update(float dt)
		{
			if (state_ != WorldState::RUNNING)
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