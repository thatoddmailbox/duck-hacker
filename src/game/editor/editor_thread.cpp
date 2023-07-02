#include "game/editor/editor_thread.hpp"

wxIMPLEMENT_APP_NO_MAIN(duckhacker::game::editor::App);

#include <unistd.h>
namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			void EditorThread::Run(EditorThread * t)
			{
				t->Run_();
			}

			void EditorThread::Run_()
			{
				int argc = 0;
				char * argv[0];
				wxEntry(argc, argv);

				// printf("beep boop i am thread\n");
				// while (1)
				// {
				// 	sleep(1);
				// 	printf("hi\n");
				// }
			}

			void EditorThread::RequestStop()
			{
				wxThreadEvent * e = new wxThreadEvent(EVENT_REQUEST_STOP);
				wxGetApp().QueueEvent(e);
			}
		}
	}
}