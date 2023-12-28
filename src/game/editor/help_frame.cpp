#include "game/editor/help_frame.hpp"

#include <wx/notebook.h>

#include "game/editor/app.hpp"

wxDECLARE_APP(duckhacker::game::editor::App);

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

				wxNotebook * notebook = new wxNotebook(panel, wxID_ANY);
				root->Add(notebook, wxSizerFlags().Expand().Proportion(1));

				wxPanel * duckbot_panel = new wxPanel(notebook, wxID_ANY);

				wxSizer * duckbot_panel_sizer = new wxBoxSizer(wxVERTICAL);
				duckbot_panel->SetSizer(duckbot_panel_sizer);

				wxTextCtrl * duckbot_text = new wxTextCtrl(
					duckbot_panel,
					wxID_ANY,
					"DuckBots are programmed in Lua. (To learn about programming in Lua, switch to the Lua programming tab.)\n"
					"\n"
					"There are many functions that can be used when programming a DuckBot. Here's a list of all of them.\n"
					"\n"
					"Movement:\n"
					"  * duckbot.moveForward()\n\tmoves the bot forward one space\n\n"
					"  * duckbot.moveBackward()\n\tmoves the bot backward one space\n\n"
					"  * duckbot.canMoveForward()\n\treturns true if the bot can move forward, false if it can't\n\n"
					"  * duckbot.canMoveBackward()\n\treturns true if the bot can move backward, false if it can't\n\n"
					"  * duckbot.turnLeft()\n\trotates the bot 90 degrees to its left\n\n"
					"  * duckbot.turnRight()\n\trotates the bot 90 degrees to its right\n\n"
					"\n"
					"Conversation:\n"
					"  * duckbot.say(\"Hello!\")\n\tsays the given message\n\n"
					"  * duckbot.listen()\n\tlistens for a message from another bot, and then returns it\n\n"
					"\n"
					"World:\n"
					"  * duckbot.getCoins()\n\treturns the number of coins you have\n\n"
					"  * duckbot.getTime()\n\treturns the current time, in ticks\n\n"
					"  * duckbot.sleep(10)\n\tsleeps for the given amount of ticks (10 ticks = 1 second)\n\n",
					wxDefaultPosition,
					wxDefaultSize,
					wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxTE_RICH2 | wxTE_AUTO_URL | wxTE_NOHIDESEL
				);
				duckbot_text->SetEditable(false);
				duckbot_text->SetBackgroundColour(wxColour(255, 255, 255));
				duckbot_text->SetMargins(10, 10);
				duckbot_panel_sizer->Add(duckbot_text, wxSizerFlags().Expand().Proportion(1));

				notebook->AddPage(duckbot_panel, "DuckBot programming");

				wxPanel * lua_panel = new wxPanel(notebook, wxID_ANY);

				wxSizer * lua_panel_sizer = new wxBoxSizer(wxVERTICAL);
				lua_panel->SetSizer(lua_panel_sizer);

				wxStaticText * lua_text = new wxStaticText(lua_panel, wxID_ANY, "Help goes here I guess?");
				lua_panel_sizer->Add(lua_text, wxSizerFlags().Center());

				notebook->AddPage(lua_panel, "Lua programming");
			}

			HelpFrame::~HelpFrame()
			{
				wxGetApp().NotifyHelpFrameClosed();
			}
		}
	}
}