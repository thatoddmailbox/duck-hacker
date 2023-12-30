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
			void OnTextURL(wxTextUrlEvent & event)
			{
				if (!event.GetMouseEvent().ButtonDown(wxMOUSE_BTN_LEFT))
				{
					return;
				}

				wxTextCtrl * text = (wxTextCtrl *) event.GetEventObject();

				wxString url = text->GetRange(event.GetURLStart(), event.GetURLEnd());

				wxLaunchDefaultBrowser(url);
			}

			HelpFrame::HelpFrame() : wxFrame(NULL, wxID_ANY, "Help", wxDefaultPosition, wxSize(500, 300))
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
					"Conversation:\n"
					"  * duckbot.say(\"Hello!\")\n\tsays the given message\n\n"
					"  * duckbot.listen()\n\tlistens for a message from another bot, and then returns it\n\n"
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
				duckbot_text->SetMargins(5, 0);
				duckbot_text->Bind(wxEVT_TEXT_URL, OnTextURL);
				duckbot_panel_sizer->Add(duckbot_text, wxSizerFlags().Expand().Proportion(1));

				notebook->AddPage(duckbot_panel, "DuckBot programming");

				wxPanel * lua_panel = new wxPanel(notebook, wxID_ANY);

				wxSizer * lua_panel_sizer = new wxBoxSizer(wxVERTICAL);
				lua_panel->SetSizer(lua_panel_sizer);

				wxTextCtrl * lua_text = new wxTextCtrl(
					lua_panel,
					wxID_ANY,
					"Lua is a simple programming language. You can find a lot of information online, but here is a short summary.\n"
					"\n"
					"Basics:\n"
					"  * -- this is a comment\n\n"
					"  * 10 + 20\n\treturns 30\n\n"
					"  * \"Hello\" .. \" world\"\n\treturns \"Hello world\"\n\n"
					"  * tonumber(\"10\")\n\treturns 10\n\n"
					"  * tostring(10)\n\treturns \"10\"\n\n"
					"  * print(\"Hello!\")\n\tprints \"Hello!\" to the console\n\n"
					"  * error(\"Quack\")\n\tthrows an error with message \"Quack\"\n\n"
					"Variables:\n"
					"  * local x = 10\n\tdeclares a variable named x, and sets it to 10\n\n"
					"  * x = 20\n\tsets the variable x to 20\n\n"
					"  * local y\n\tdeclares a variable named y, but doesn't set it to anything\n\n"
					"  * local z = x + y\n\tsets the variable z to the sum of x and y\n\n"
					"Functions:\n"
					"  * function foo()\n    \t-- do stuff\n    end\n\n"
					"  * function bar(x, y)\n    \treturn x + y\n    end\n\n"
					"Conditionals:\n"
					"  * if x == 10 then\n    \t-- do stuff\n    end\n\n"
					"  * if x == 10 then\n    \t-- do stuff\n    else\n    \t-- do other stuff\n    end\n\n"
					"  * if x == 10 then\n    \t-- do stuff\n    elseif x == 20 then\n    \t-- do other stuff\n    else\n    \t-- do other other stuff\n    end\n\n"
					"Loops:\n"
					"  * while x < 10 do -- while x is less than 10, do...\n    \t-- do stuff\n    end\n\n"
					"  * for i = 1, 10 do -- for i from 1 to 10, do...\n    \t-- do stuff\n    end\n\n"
					"Arrays:\n"
					"  * NOTE: Lua arrays start at 1, not 0\n\n"
					"  * local a = {}\n\tdeclares an empty array named a\n\n"
					"  * local a = {1, 2, 3}\n\tdeclares an array named a with three elements\n\n"
					"  * a[1] = 10\n\tsets the first element of a to 10\n\n"
					"  * #a\n\treturns the number of elements in a\n\n"
					"  * table.insert(a, 10)\n\tadds 10 to the end of a\n\n"
					"  * table.remove(a)\n\tremoves the last element of a\n\n"
					"  * table.remove(a, 2)\n\tremoves the second element of a\n\n"
					"You may also find this reference of built-in Lua functions useful: https://www.lua.org/manual/5.4/#functions\n"
					"DuckBots can access all the math, string, and table functions."
					,
					wxDefaultPosition,
					wxDefaultSize,
					wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxTE_RICH2 | wxTE_AUTO_URL | wxTE_NOHIDESEL
				);
				lua_text->SetEditable(false);
				lua_text->SetBackgroundColour(wxColour(255, 255, 255));
				lua_text->SetMargins(5, 0);
				lua_text->Bind(wxEVT_TEXT_URL, OnTextURL);
				lua_panel_sizer->Add(lua_text, wxSizerFlags().Expand().Proportion(1));

				notebook->AddPage(lua_panel, "Lua programming");
			}

			HelpFrame::~HelpFrame()
			{
				wxGetApp().NotifyHelpFrameClosed();
			}
		}
	}
}