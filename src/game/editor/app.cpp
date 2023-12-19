#include "game/editor/app.hpp"

#include "game/editor/frame.hpp"
#include "game/editor/help_frame.hpp"

wxDEFINE_EVENT(EVENT_REQUEST_STOP, wxThreadEvent);
wxDEFINE_EVENT(EVENT_OPEN_EDITOR, wxThreadEvent);
wxDEFINE_EVENT(EVENT_GATHER_CODE, wxThreadEvent);

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
				Bind(EVENT_OPEN_EDITOR, &App::OnOpenEditor, this);
				Bind(EVENT_GATHER_CODE, &App::OnGatherCode, this);

				// wxMessageBox("Hellooooo this is wxWidgets!!","hi :)", wxOK | wxICON_INFORMATION, nullptr);

				// wxString code = "duck";
				// Frame * frame = new Frame(1, code);
				// frame->Show(true);
				// frames_[1] = frame;

				return true;
			}

			void App::OpenHelp()
			{
				if (help_frame_ == nullptr)
				{
					help_frame_ = new HelpFrame();
					help_frame_->Show(true);
				}
				else
				{
					help_frame_->SetFocus();
				}
			}

			void App::OnOpenEditor(wxThreadEvent& e)
			{
				int bot_id = e.GetInt();
				wxString initial_code = e.GetString();

				if (frames_.find(bot_id) == frames_.end())
				{
					// no existing editor for this bot
					// open a new one
					Frame * frame = new Frame(bot_id, initial_code);
					frame->Show(true);
					frames_[bot_id] = frame;
				}
				else
				{
					// there already is an editor, bring it to front
					frames_[bot_id]->Focus();
				}
			}

			void App::OnGatherCode(wxThreadEvent& e)
			{
				GatherCodeContext * ctx = e.GetPayload<GatherCodeContext *>();

				std::unique_lock<std::mutex> lock(ctx->result_mutex);

				for (std::pair<int, Frame *> p : frames_)
				{
					ctx->result[p.first] = p.second->GetCode();
				}

				ctx->done = true;

				lock.unlock();
				ctx->result_notify.notify_one();
			}

			void App::OnRequestStop(wxThreadEvent& e)
			{
				ExitMainLoop();
			}
		}
	}
}