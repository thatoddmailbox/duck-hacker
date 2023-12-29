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

			bool IsDebuggingEnabled();

			void LoadProgress();
			void SaveProgress();

			void SetLevelCompleted(std::string path);

			int GetLevelIndexFromPath(std::string path);
			std::string GetLevelName(int index);
			const std::string& GetLevelPath(int index);
			int GetLevelCount();
			bool IsLevelUnlocked(int index);

		private:
			bool debugging_enabled_;
			std::vector<bool> completed_;
			std::string progress_path_;
		};
	}
}

#endif