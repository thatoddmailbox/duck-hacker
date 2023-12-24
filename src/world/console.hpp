#ifndef _WORLD_CONSOLE_HPP
#define _WORLD_CONSOLE_HPP

#include <mutex>
#include <vector>

#include "world/bot.hpp"

namespace duckhacker
{
	namespace world
	{
		struct ConsoleLine
		{
			std::string name;
			std::string text;
		};

		class Console
		{
		public:
			const std::vector<ConsoleLine>& LockLines();
			void UnlockLines();

			void Update(std::vector<Bot *>& bots);

		private:
			std::mutex lines_mutex_;
			std::vector<ConsoleLine> lines_;
		};
	}
}

#endif
