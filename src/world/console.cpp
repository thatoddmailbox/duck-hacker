#include "world/console.hpp"

namespace duckhacker
{
	namespace world
	{
		const std::vector<ConsoleLine>& Console::LockLines()
		{
			lines_mutex_.lock();
			return lines_;
		}

		void Console::UnlockLines()
		{
			lines_mutex_.unlock();
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

					lines_mutex_.lock();
					lines_.push_back(line);
					lines_mutex_.unlock();
				}
			}
		}
	}
}