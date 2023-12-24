#ifndef _WORLD_CONSOLE_LINE_HPP
#define _WORLD_CONSOLE_LINE_HPP

#include <string>

namespace duckhacker
{
	namespace world
	{
		enum class ConsoleLineType
		{
			INFO,
			WARNING,
			ERROR
		};

		constexpr const char * ConsoleLineTypeToString[] =
		{
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
