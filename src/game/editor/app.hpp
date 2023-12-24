#ifndef _GAME_EDITOR_APP_HPP
#define _GAME_EDITOR_APP_HPP

#include <condition_variable>
#include <map>
#include <mutex>

#include <wx/wx.h>

wxDECLARE_EVENT(EVENT_REQUEST_STOP, wxThreadEvent);
wxDECLARE_EVENT(EVENT_OPEN_EDITOR, wxThreadEvent);
wxDECLARE_EVENT(EVENT_GATHER_CODE, wxThreadEvent);

namespace duckhacker
{
	namespace game
	{
		namespace editor
		{
			struct GatherCodeContext
			{
				std::map<int, std::string> result;
				std::mutex result_mutex;
				std::condition_variable result_notify;
				bool done = false;
			};

			class Frame;
			class HelpFrame;

			class App : public wxApp
			{
			public:
				virtual bool OnInit() wxOVERRIDE;

				void OpenHelp();

			protected:
				friend class Frame;
				void NotifyFrameClosed(Frame * frame);

			private:
				void OnOpenEditor(wxThreadEvent& e);
				void OnRequestStop(wxThreadEvent& e);
				void OnGatherCode(wxThreadEvent& e);

				std::map<int, Frame *> frames_;
				HelpFrame * help_frame_ = nullptr;
			};
		}
	}
}

#endif