#include "game/editor/help_frame.hpp"

namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			HelpFrame::HelpFrame() : wxFrame(NULL, wxID_ANY, "Help")
			{
				wxSizer * panel_base = new wxBoxSizer(wxVERTICAL);
				wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
				panel_base->Add(panel, 1, wxEXPAND);
				SetSizer(panel_base);

				wxSizer * root = new wxBoxSizer(wxVERTICAL);
				panel->SetSizer(root);

				wxStaticText * text = new wxStaticText(panel, wxID_ANY, "Help goes here I guess?");
				root->Add(text, wxSizerFlags().Center());
			}
		}
	}
}