#include "game/editor/help_frame.hpp"

namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			HelpFrame::HelpFrame() : wxFrame(NULL, wxID_ANY, "Help")
			{
				wxSizer * root = new wxBoxSizer(wxVERTICAL);

				wxStaticText * text = new wxStaticText(this, wxID_ANY, "Help goes here I guess?");
				root->Add(text, wxSizerFlags().Center());

				SetSizer(root);
			}
		}
	}
}