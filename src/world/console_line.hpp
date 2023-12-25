#ifndef _WORLD_CONSOLE_LINE_HPP
#define _WORLD_CONSOLE_LINE_HPP

#include <string>

namespace duckhacker
{
	namespace world
	{
		enum class ConsoleLineType
		{
			SPOKEN,
			INFO,
			WARNING,
			ERR
		};

		constexpr const char * ConsoleLineTypeToString[] =
		{
			"S",
			"I",
			"W",
			"E"
		};

		struct ConsoleLine
		{
			int ticks;
			ConsoleLineType type;
			std::string name;
			std::string text;
		};
	}
}

#endif
