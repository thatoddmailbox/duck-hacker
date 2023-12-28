#ifndef _PROGRESS_MANAGER_HPP
#define _PROGRESS_MANAGER_HPP

#include <string>

namespace duckhacker
{
	class Game;

	namespace progress
	{
		class Manager
		{
		public:
			void Init();

			std::string GetLevelName(int index);
			const std::string& GetLevelPath(int index);
			int GetLevelCount();
			bool IsLevelUnlocked(int index);
		};
	}
}

#endif