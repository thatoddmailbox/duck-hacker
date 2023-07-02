#ifndef _GAME_EDITOR_EDITOR_THREAD_HPP
#define _GAME_EDITOR_EDITOR_THREAD_HPP

#include <wx/wx.h>

#include "game/editor/app.hpp"

namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			class EditorThread
			{
			public:
				static void Run(EditorThread * t);
				void RequestStop();

			private:
				void Run_();
			};
		}
	}
}

#endif