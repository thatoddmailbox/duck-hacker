#ifndef _WORLD_CONSOLE_HPP
#define _WORLD_CONSOLE_HPP

#include <mutex>
#include <vector>

#include "world/bot.hpp"
#include "world/console_line.hpp"

namespace duckhacker
{
	namespace world
	{
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
