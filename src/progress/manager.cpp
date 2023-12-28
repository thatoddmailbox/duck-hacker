#include "progress/manager.hpp"

#include <vector>

std::vector<std::string> levels = {
	"worlds/level1.xml",
	"worlds/level2.xml",
	"worlds/level3.xml",
	"worlds/level4.xml",
	"worlds/level5.xml",
	"worlds/level6.xml",
};

namespace duckhacker
{
	namespace progress
	{
		void Manager::Init()
		{
			// TODO; load from disk? or something?
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

			// TODO: implement
			return false;
		}
	}
}