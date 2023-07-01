#ifndef _WORLD_BOT_HPP
#define _WORLD_BOT_HPP

#include <string>

namespace duckhacker
{
	namespace world
	{
		class Bot
		{
		public:
			Bot();

			int id;
			int x, y, z;
			std::string code;
		};
	}
}

#endif