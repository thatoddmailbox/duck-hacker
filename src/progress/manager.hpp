#ifndef _PROGRESS_MANAGER_HPP
#define _PROGRESS_MANAGER_HPP

#include <string>
#include <vector>

namespace duckhacker
{
	class Game;

	namespace progress
	{
		class Manager
		{
		public:
			void Init();

			void LoadProgress();
			void SaveProgress();

			void SetLevelCompleted(std::string path);

			int GetLevelIndexFromPath(std::string path);
			std::string GetLevelName(int index);
			const std::string& GetLevelPath(int index);
			int GetLevelCount();
			bool IsLevelUnlocked(int index);

		private:
			std::vector<bool> completed_;
			std::string progress_path_;
		};
	}
}

#endif