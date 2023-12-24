#include "world/console.hpp"

#define CONSOLE_MAX_LINES 300

namespace duckhacker
{
	namespace world
	{
		void Console::Reset()
		{
			lines_.clear();
		}

		const std::deque<ConsoleLine>& Console::GetLines()
		{
			return lines_;
		}

		void Console::Update(std::vector<Bot *>& bots)
		{
			for (Bot * bot : bots)
			{
				std::unique_lock<std::mutex> lock(bot->lines_mutex_);
				while (bot->lines_.size() > 0)
				{
					ConsoleLine line = bot->lines_.front();
					bot->lines_.pop_front();

					lines_.push_back(line);
					if (lines_.size() > CONSOLE_MAX_LINES)
					{
						lines_.pop_front();
					}

					new_lines = true;
				}
			}
		}
	}
}