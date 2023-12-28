#include "music/manager.hpp"

#include <SDL.h>

namespace duckhacker
{
	namespace music
	{
		void Manager::Init()
		{
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
			{
				std::string message = "Failed to initialize audio: " + std::string(Mix_GetError());
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", message.c_str(), nullptr);
			}
		}

		void Manager::PlayTrack(std::string track)
		{
			// TODO: implement
		}
	}
}