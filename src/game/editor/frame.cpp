#include "game/editor/frame.hpp"

namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			Frame::Frame(int bot_id, wxString& initial_code) : wxFrame(NULL, wxID_ANY, "Code Editor")
			{
				SetTitle("Code Editor (DuckBot " + std::to_string(bot_id) + ")");

				wxSizer * root = new wxBoxSizer(wxVERTICAL);

				wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
				sizer->AddSpacer(10);

				// wxStaticText * bot_info = new wxStaticText(this, wxID_ANY, "DuckBot " + std::to_string(bot_id));
				// sizer->Add(bot_info, wxSizerFlags().Center());

				sizer->AddStretchSpacer(1);

				wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
				// aboutBtn->Bind(wxEVT_BUTTON, &MyFrame::OnAbout, this);
				sizer->Add(aboutBtn, wxSizerFlags().Center());

				sizer->AddSpacer(4);

				wxButton* quackBtn = new wxButton(this, wxID_ANY, "Quack!");
				// quackBtn->Bind(wxEVT_BUTTON, &MyFrame::OnQuack, this);
				sizer->Add(quackBtn, wxSizerFlags().Center());

				sizer->AddSpacer(10);

				root->AddSpacer(2);
				root->Add(sizer, 0, wxEXPAND);
				root->AddSpacer(2);

				text_control_ = new wxStyledTextCtrl(this, wxID_ANY);

				text_control_->SetMarginWidth(0, 32);
				text_control_->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75) );
				text_control_->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
				text_control_->SetMarginType(0, wxSTC_MARGIN_NUMBER);

				text_control_->SetWrapMode(wxSTC_WRAP_WORD);

				text_control_->SetText(initial_code);

				wxFont monospace = wxFontInfo().Family(wxFONTFAMILY_TELETYPE);
				text_control_->StyleSetFont(wxSTC_STYLE_DEFAULT, monospace);
				text_control_->SetFont(monospace);

				text_control_->StyleClearAll();
				text_control_->SetLexer(wxSTC_LEX_LUA);
				text_control_->StyleSetForeground(wxSTC_LUA_STRING,     wxColour(255,0,0));
				text_control_->StyleSetForeground(wxSTC_LUA_CHARACTER,     wxColour(255,0,0));
				text_control_->StyleSetForeground(wxSTC_LUA_IDENTIFIER,           wxColour(255,0,255));
				text_control_->StyleSetForeground(wxSTC_LUA_WORD,              wxColour(0,150,0));
				text_control_->StyleSetForeground(wxSTC_LUA_NUMBER,        wxColour(0,0,150));
				text_control_->StyleSetForeground(wxSTC_LUA_COMMENT,          wxColour(150,150,150));
				text_control_->StyleSetForeground(wxSTC_LUA_COMMENTLINE,          wxColour(150,150,150));

				root->Add(text_control_, 1, wxEXPAND);
				// printf("text %p\n", text);

				SetSizer(root);
			}

			wxString Frame::GetCode()
			{
				return text_control_->GetText();
			}

			void Frame::SetCode(wxString code)
			{
				text_control_->SetText(code);
			}

			void Frame::Focus()
			{
				SetFocus();

				// TODO: focus on text control, too
				// text_control_->SetFocus();
			}
		}
	}
}