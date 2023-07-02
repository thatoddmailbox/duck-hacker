#include "game/editor/app.hpp"

#include "game/editor/frame.hpp"

wxDEFINE_EVENT(EVENT_REQUEST_STOP, wxThreadEvent);

namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			bool App::OnInit()
			{
				if (!wxApp::OnInit())
				{
					return false;
				}

				SetExitOnFrameDelete(false);

				Bind(EVENT_REQUEST_STOP, &App::OnRequestStop, this);

				// wxMessageBox("Hellooooo this is wxWidgets!!","hi :)", wxOK | wxICON_INFORMATION, nullptr);

				Frame * frame = new Frame();
				frame->Show(true);

				return true;
			}

			void App::OnRequestStop(wxThreadEvent& e)
			{
				ExitMainLoop();
			}

			// wxBEGIN_EVENT_TABLE(App, wxApp))
			// 	EVT_COMMAND(wxID_ANY, MY_NEW_TYPE, App::OnMyEvent)
			// wxEND_EVENT_TABLE()
		}
	}
}