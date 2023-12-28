#ifndef _MUSIC_MANAGER_HPP
#define _MUSIC_MANAGER_HPP

#include <SDL_mixer.h>

#include <string>

namespace duckhacker
{
	class Game;

	namespace music
	{
		class Manager
		{
		public:
			void Init();
			void PlayTrack(std::string track);
		};
	}
}

#endif