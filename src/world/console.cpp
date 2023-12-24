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
					std::string line = bot->lines_.front();
					bot->lines_.pop_front();

					ConsoleLine console_line;
					console_line.name = bot->GetName();
					console_line.text = line;

					lines_mutex_.lock();
					lines_.push_back(console_line);
					lines_mutex_.unlock();
				}
			}
		}
	}
}