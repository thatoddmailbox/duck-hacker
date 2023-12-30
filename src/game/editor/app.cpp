#include "game/editor/app.hpp"

#include "game/editor/frame.hpp"
#include "game/editor/help_frame.hpp"

#include "world/bot.hpp"
#include "world/world.hpp"

wxDEFINE_EVENT(EVENT_REQUEST_STOP, wxThreadEvent);
wxDEFINE_EVENT(EVENT_OPEN_EDITOR, wxThreadEvent);
wxDEFINE_EVENT(EVENT_GATHER_CODE, wxThreadEvent);
wxDEFINE_EVENT(EVENT_SET_WORLD, wxThreadEvent);

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
				Bind(EVENT_SET_WORLD, &App::OnSetWorld, this);

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

			void App::NotifyFrameClosed(Frame * frame)
			{
				for (std::pair<int, Frame *> p : frames_)
				{
					if (p.second == frame)
					{
						int bot_id = p.first;

						world::Bot * bot = bots_[bot_id];
						bot->SetCode(std::string(p.second->GetCode()));

						frames_.erase(bot_id);
						bots_.erase(bots_.find(bot_id));

						break;
					}
				}
			}

			void App::NotifyHelpFrameClosed()
			{
				help_frame_ = nullptr;
			}

			void App::OnOpenEditor(wxThreadEvent& e)
			{
				world::Bot * bot = e.GetPayload<world::Bot *>();

				int bot_id = bot->GetID();

				if (frames_.find(bot_id) == frames_.end())
				{
					bots_[bot_id] = bot;

					// no existing editor for this bot
					// open a new one
					wxString initial_code = bot->GetCode();
					Frame * frame = new Frame(bot_id, bot->GetName(), initial_code);
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

			void App::OnSetWorld(wxThreadEvent& e)
			{
				SetWorldContext * ctx = e.GetPayload<SetWorldContext*>();

				// close any open windows
				// note how we build a list and then iterate over that, instead of iterating over the map directly
				// this is because when a window is closed, it will call NotifyFrameClosed, which will remove the window from the map
				// and we don't want to modify the map while iterating over it

				std::vector<Frame *> frames_to_close;
				for (std::pair<int, Frame *> p : frames_)
				{
					frames_to_close.push_back(p.second);
				}

				for (Frame * frame : frames_to_close)
				{
					frame->Close();
					delete frame;
				}

				frames_.clear();
				bots_.clear();

				ctx->done_mutex.lock();
				ctx->done = true;
				ctx->done_mutex.unlock();

				ctx->done_notify.notify_one();

			}
		}
	}
}