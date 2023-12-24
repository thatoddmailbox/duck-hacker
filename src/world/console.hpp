#ifndef _WORLD_CONSOLE_HPP
#define _WORLD_CONSOLE_HPP

#include <atomic>
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

			std::atomic_bool new_lines = ATOMIC_VAR_INIT(false);

			void Update(std::vector<Bot *>& bots);

		private:
			std::mutex lines_mutex_;
			std::vector<ConsoleLine> lines_;
		};
	}
}

#endif
