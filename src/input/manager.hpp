#ifndef _INPUT_MANAGER_HPP
#define _INPUT_MANAGER_HPP

#include <SDL.h>

#include "external/imgui/imgui.h"

namespace duckhacker
{
	class Game;

	namespace input
	{
		enum Axis
		{
			Horizontal,
			Vertical,

			MouseX,
			MouseY,

			MouseScrollX,
			MouseScrollY
		};

		class Manager
		{
		public:
			double GetAxis(Axis axis);

		protected:
			void EarlyUpdate();
			void ProcessEvent(SDL_Event * e);
			void Update();

			friend class duckhacker::Game;

		private:
			int mouse_x_last_, mouse_y_last_;
			bool mouse_left_down_last_;

			int horizontal_, vertical_;

			int mouse_scroll_x_, mouse_scroll_y_;
		};
	}
}

#endif