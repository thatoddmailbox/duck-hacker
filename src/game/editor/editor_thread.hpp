#ifndef _GAME_EDITOR_EDITOR_THREAD_HPP
#define _GAME_EDITOR_EDITOR_THREAD_HPP

#include <map>
#include <mutex>

#include <wx/wx.h>

#include "game/editor/app.hpp"

namespace duckhacker
{
	namespace world
	{
		class Bot;
		class World;
	}

	namespace game
	{
		namespace editor
		{
			class EditorThread
			{
			public:
				static void Run(EditorThread * t);
				void SetWorld(world::World * world);
				void OpenEditor(world::Bot * bot);
				std::map<int, std::string> GatherCode();
				void RequestStop();

			private:
				void Run_();
			};
		}
	}
}

#endif