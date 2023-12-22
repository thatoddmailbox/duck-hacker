#ifndef _GAME_EDITOR_FRAME_HPP
#define _GAME_EDITOR_FRAME_HPP

#include <wx/wx.h>
#include <wx/stc/stc.h>

namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			class Frame : public wxFrame
			{
			public:
				Frame(int bot_id, wxString& initial_code);
				~Frame();

				wxString GetCode();
				void SetCode(wxString code);

				void Focus();

			private:
				void OnOpenHelp(wxCommandEvent& e);

				wxStyledTextCtrl * text_control_ = nullptr;
			};
		}
	}
}

#endif