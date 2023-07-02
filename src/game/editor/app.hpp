#ifndef _GAME_EDITOR_APP_HPP
#define _GAME_EDITOR_APP_HPP

#include <wx/wx.h>

wxDECLARE_EVENT(EVENT_REQUEST_STOP, wxThreadEvent);

namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			class App : public wxApp
			{
			public:
				virtual bool OnInit() wxOVERRIDE;

			private:
				void OnRequestStop(wxThreadEvent& e);
			};
		}
	}
}

#endif