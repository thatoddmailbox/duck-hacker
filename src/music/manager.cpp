#include "music/manager.hpp"

#include <SDL.h>

namespace duckhacker
{
	namespace music
	{
		void Manager::Init(content::Manager * content_manager)
		{
			content_manager_ = content_manager;

			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
			{
				std::string message = "Failed to initialize audio: " + std::string(Mix_GetError());
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", message.c_str(), nullptr);
			}
		}

		void Manager::PlaySoundEffect(std::string sound_effect)
		{
			Mix_Chunk * chunk = content_manager_->SoundEffect("sounds/" + sound_effect + ".wav");
			Mix_PlayChannel(-1, chunk, 0);
		}

		void Manager::PlayTrack(std::string track)
		{
			if (track == current_track_)
			{
				return;
			}

			Mix_Music * music = content_manager_->Music("music/" + track + ".ogg");
			Mix_FadeOutMusic(50);
			Mix_FadeInMusic(music, -1, 200);
			current_track_ = track;
		}
	}
}