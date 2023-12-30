#include "progress/manager.hpp"

#include <fstream>

#include <SDL.h>

#include "pugixml.hpp"

std::vector<std::string> levels = {
	"intro",
	"worlds/level1.xml",
	"worlds/level2.xml",
	"worlds/level3.xml",
	"worlds/level4.xml",
	"worlds/level5.xml",
	"worlds/level6.xml",
	"outro",
	"credits"
};

namespace duckhacker
{
	namespace progress
	{
		void Manager::Init()
		{
			char * path = SDL_GetPrefPath("Dogo", "Duck Hacker");

			progress_path_ = std::string(path) + "progress.xml";
			std::string debug_path = std::string(path) + "debug.txt";

			SDL_free(path);

			debugging_enabled_ = std::ifstream(debug_path).good();

			LoadProgress();
		}

		bool Manager::IsDebuggingEnabled()
		{
			return debugging_enabled_;
		}

		void Manager::LoadProgress()
		{
			for (int i = 0; i < levels.size(); i++)
			{
				completed_.push_back(false);
			}

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(progress_path_.c_str());
			if (!result)
			{
				// assume no progress
				return;
			}

			pugi::xml_node root = doc.child("progress");
			for (pugi::xml_node level = root.child("level"); level; level = level.next_sibling("level"))
			{
				std::string path = level.attribute("path").as_string();
				bool completed = level.attribute("completed").as_bool();

				int index = GetLevelIndexFromPath(path);

				completed_[index] = completed;
			}
		}

		void Manager::SaveProgress()
		{
			pugi::xml_document doc;
			pugi::xml_node root = doc.append_child("progress");

			for (int i = 0; i < levels.size(); i++)
			{
				pugi::xml_node level = root.append_child("level");
				level.append_attribute("path").set_value(levels[i].c_str());
				level.append_attribute("completed").set_value(completed_[i]);
			}

			doc.save_file(progress_path_.c_str());
		}

		void Manager::SetLevelCompleted(std::string path)
		{
			int index = GetLevelIndexFromPath(path);
			completed_[index] = true;
			SaveProgress();
		}

		int Manager::GetLevelIndexFromPath(std::string path)
		{
			for (int i = 0; i < levels.size(); i++)
			{
				if (levels[i] == path)
				{
					return i;
				}
			}

			return -1;
		}

		std::string Manager::GetLevelName(int index)
		{
			if (index == 0)
			{
				return "Intro";
			}
			else if (index == levels.size() - 2)
			{
				return "Outro";
			}
			else if (index == levels.size() - 1)
			{
				return "Credits";
			}

			return "Level " + std::to_string(index);
		}

		const std::string& Manager::GetLevelPath(int index)
		{
			return levels[index];
		}

		int Manager::GetLevelCount()
		{
			return levels.size();
		}

		bool Manager::IsLevelUnlocked(int index)
		{
			if (index == 0)
			{
				return true;
			}

			return completed_[index - 1];
		}
	}
}