#include "game/editor/editor_thread.hpp"

wxIMPLEMENT_APP_NO_MAIN(duckhacker::game::editor::App);

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
				char * argv[1];
				argv[0] = nullptr;
				wxEntry(argc, argv);
			}

			void EditorThread::SetWorld(world::World * world)
			{
				wxThreadEvent * e = new wxThreadEvent(EVENT_SET_WORLD);
				e->SetPayload(world);
				wxGetApp().QueueEvent(e);
			}

			void EditorThread::OpenEditor(world::Bot * bot)
			{
				wxThreadEvent * e = new wxThreadEvent(EVENT_OPEN_EDITOR);
				e->SetPayload(bot);
				wxGetApp().QueueEvent(e);
			}

			std::map<int, std::string> EditorThread::GatherCode()
			{
				GatherCodeContext gather_code_context;

				wxThreadEvent * e = new wxThreadEvent(EVENT_GATHER_CODE);
				e->SetPayload(&gather_code_context);
				wxGetApp().QueueEvent(e);

				{
					std::unique_lock<std::mutex> lock(gather_code_context.result_mutex);
					gather_code_context.result_notify.wait(lock, [&gather_code_context] {
						return gather_code_context.done;
					});
				}

				return gather_code_context.result;
			}

			void EditorThread::RequestStop()
			{
				wxThreadEvent * e = new wxThreadEvent(EVENT_REQUEST_STOP);
				wxGetApp().QueueEvent(e);
			}
		}
	}
}