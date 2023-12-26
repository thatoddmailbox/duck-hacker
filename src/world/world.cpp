#include "world/world.hpp"

#include "render/mesh_factory.hpp"

#include "pugixml.hpp"

namespace duckhacker
{
	namespace world
	{
		World::World(content::Manager * content_manager, std::string file_path)
		{
			int64_t file_length;
			char * file_contents = content_manager->File(file_path, &file_length);

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_buffer(file_contents, file_length);
			free(file_contents);

			if (!result)
			{
				std::cout << "Failed to load world." << std::endl;
				return;
			}

			pugi::xml_node world_node = doc.child("world");

			initial_coins_ = world_node.attribute("coins").as_int();
			coins_ = initial_coins_;

			pugi::xml_node field_node = world_node.child("field");
			int field_width = field_node.attribute("width").as_int();
			float field_height = 4;
			int field_depth = field_node.attribute("depth").as_int();

			// required so the walls cover the floor
			float field_height_offset = 1.5;

			center_point_ = glm::vec3(field_width / 2, 0, field_depth / 2);

			render::Object floor = render::Object();
			floor.SetMesh(render::MeshFactory::Box(content_manager->Shader("shaders/basic"), field_width, 1, field_depth));
			floor.SetPosition(glm::vec3(field_width / 2, -1, field_depth / 2));
			objects.push_back(floor);

			render::Object wall1 = render::Object();
			wall1.SetMesh(render::MeshFactory::Box(content_manager->Shader("shaders/basic"), field_width, field_height, 1));
			wall1.SetPosition(glm::vec3(field_width / 2, (field_height / 2) - field_height_offset, field_depth));
			objects.push_back(wall1);

			render::Object wall2 = render::Object();
			wall2.SetMesh(render::MeshFactory::Box(content_manager->Shader("shaders/basic"), 1, field_height, field_depth));
			wall2.SetPosition(glm::vec3(field_width, (field_height / 2) - field_height_offset, field_depth / 2));
			objects.push_back(wall2);

			pugi::xml_node bots_node = world_node.child("bots");

			for (pugi::xml_node bot_node : bots_node.children("bot"))
			{
				int id = bot_node.attribute("id").as_int();
				std::string name = bot_node.attribute("name").as_string();
				int x = bot_node.attribute("x").as_int();
				int y = bot_node.attribute("y").as_int();
				int z = bot_node.attribute("z").as_int();
				int rotation = bot_node.attribute("rotation").as_int();
				std::string mesh = bot_node.attribute("mesh").as_string();
				std::string src = bot_node.attribute("src").as_string();

				std::string type_string = bot_node.attribute("type").as_string();
				BotType type = BotType::PLAYER;
				if (type_string == "npc")
				{
					type = BotType::NPC;
				}

				std::string code;
				if (!src.empty())
				{
					char * code_file_data = content_manager->File(src, nullptr);
					code = std::string(code_file_data);
					free(code_file_data);
				}

				Bot * bot = new Bot(this, content_manager, type, id, name, x, y, z, rotation, mesh, code);
				bots.push_back(bot);

				bot_reset_positions_.push_back(glm::vec4(x, y, z, rotation));
			}

			pugi::xml_node objects_node = world_node.child("objects");

			for (pugi::xml_node object_node : objects_node.children("object"))
			{
				pugi::xml_node mesh_node = object_node.child("mesh");
				std::string mesh_type = mesh_node.attribute("type").as_string();

				render::Shader * shader = content_manager->Shader("shaders/basic");
				render::Mesh * mesh = nullptr;

				if (mesh_type == "cube" || mesh_type == "sign")
				{
					float width = mesh_node.attribute("w").as_float();
					float height = mesh_node.attribute("h").as_float();
					float depth = mesh_node.attribute("d").as_float();

					mesh = render::MeshFactory::Box(shader, width, height, depth);
				}
				else if (mesh_type == "obj")
				{
					std::string path = mesh_node.attribute("src").as_string();
					mesh = render::MeshFactory::OBJ(content_manager, shader, path.c_str());
				}
				else
				{
					std::cout << "Unknown mesh type: " << mesh_type << std::endl;
				}

				float x = object_node.attribute("x").as_float();
				float y = object_node.attribute("y").as_float();
				float z = object_node.attribute("z").as_float();
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
						m.SetTexture(content_manager->Texture(texture_node.attribute("src").as_string()));
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
					else if (mesh_type == "sign")
					{
						render::Material sign_material = materials[0];
						render::Material back_material = (materials.size() >= 2 ? materials[1] : render::Material());

						std::vector<std::tuple<size_t, render::Material>> material_map;

						material_map.push_back(std::make_tuple(6, sign_material));
						material_map.push_back(std::make_tuple(30, back_material));

						mesh->SetMaterials(material_map);
					}
				}

				render::Object object = render::Object();
				object.SetMesh(mesh);
				object.SetPosition(glm::vec3(x, y, z));
				object.SetRotation(glm::vec3(0, -rotation, 0));
				objects.push_back(object);
			}

			pugi::xml_node mission_node = world_node.child("mission");

			mission_ = mission_node.text().as_string();
			mission_goal_ = mission_node.attribute("goal").as_string();

			// remove tab characters
			mission_.erase(std::remove(mission_.begin(), mission_.end(), '\t'), mission_.end());

			// trim leading whitespace
			mission_.erase(mission_.begin(), std::find_if(mission_.begin(), mission_.end(), [](int ch) {
				return !std::isspace(ch);
			}));
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

		world::Console& World::GetConsole()
		{
			return console_;
		}

		const std::string& World::GetMission()
		{
			return mission_;
		}

		const std::string& World::GetMissionGoal()
		{
			return mission_goal_;
		}

		const State& World::GetState()
		{
			return state_;
		}

		const std::atomic_int& World::GetCoins()
		{
			return coins_;
		}

		const std::atomic_int& World::GetTicks()
		{
			return ticks_;
		}

		const std::map<std::string, int>& World::LockInventory()
		{
			inventory_mutex_.lock();
			return inventory_;
		}

		void World::UnlockInventory()
		{
			inventory_mutex_.unlock();
		}

		int World::GetItemCount(std::string item)
		{
			std::unique_lock<std::mutex> lock(inventory_mutex_);

			if (inventory_.find(item) == inventory_.end())
			{
				return 0;
			}

			return inventory_[item];
		}

		void World::AddCoins(int amount)
		{
			coins_ += amount;
		}

		void World::AddItem(int amount, std::string item)
		{
			std::unique_lock<std::mutex> lock(inventory_mutex_);

			if (inventory_.find(item) == inventory_.end())
			{
				inventory_[item] = amount;
			}
			else
			{
				inventory_[item] += amount;
			}
		}

		void World::Win()
		{
			state_ = State::VICTORY;
		}

		void World::Heard(int x, int y, int z, std::string message)
		{
			for (Bot * bot : bots)
			{
				if (bot->GetX() == x && bot->GetY() == y && bot->GetZ() == z)
				{
					bot->Heard(message);
					return;
				}
			}
		}

		bool World::IsOccupied(int x, int y, int z)
		{
			// TODO: should check objects, edge of field

			// never allow bots to go below y = 0
			if (y < 0)
			{
				return true;
			}

			// check bots
			for (Bot * bot : bots)
			{
				if (bot->GetX() == x && bot->GetY() == y && bot->GetZ() == z)
				{
					return true;
				}
			}

			return false;
		}

		void World::Run()
		{
			state_ = State::RUNNING;

			console_.Reset(bots);

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
		}

		void World::Reset()
		{
			// reset world
			for (size_t i = 0; i < bots.size(); i++)
			{
				Bot * bot = bots[i];
				bot->ResetTo(bot_reset_positions_[i].x, bot_reset_positions_[i].y, bot_reset_positions_[i].z, bot_reset_positions_[i].w);
			}

			state_ = State::READY;

			// reset coins and time
			coins_ = initial_coins_;
			ticks_ = 0;
			ticks_accum_ = 0;

			// clear inventory
			inventory_mutex_.lock();
			inventory_.clear();
			inventory_mutex_.unlock();
		}

		void World::Update(float dt)
		{
			if (state_ != State::RUNNING)
			{
				return;
			}

			ticks_accum_ += dt;
			while (ticks_accum_ > 0.1f)
			{
				ticks_++;
				ticks_accum_ -= 0.1f;
			}

			for (Bot * bot : bots)
			{
				bot->Update(dt);
			}

			console_.Update(bots);
		}
	}
}