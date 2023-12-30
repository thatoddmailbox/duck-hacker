#ifndef _MUSIC_MANAGER_HPP
#define _MUSIC_MANAGER_HPP

#include <SDL_mixer.h>

#include <string>

#include "content/manager.hpp"

namespace duckhacker
{
	class Game;

	namespace music
	{
		class Manager
		{
		public:
			void Init(content::Manager * content_manager);
			void PlayTrack(std::string track);

		private:
			content::Manager * content_manager_ = nullptr;
			std::string current_track_ = "";
		};
	}
}

#endif