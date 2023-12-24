#ifndef _WORLD_CONSOLE_HPP
#define _WORLD_CONSOLE_HPP

#include <atomic>
#include <deque>
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
			void Reset();

			const std::deque<ConsoleLine>& GetLines();

			std::atomic_bool new_lines = ATOMIC_VAR_INIT(false);

			void Update(std::vector<Bot *>& bots);

		private:
			std::deque<ConsoleLine> lines_;
		};
	}
}

#endif
