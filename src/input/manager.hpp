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

		enum Button
		{
			Continue
		};

		class Manager
		{
		public:
			Manager();

			double GetAxis(Axis axis);

			bool WasButtonPressed(Button button);

		protected:
			void EarlyUpdate();
			void ProcessEvent(SDL_Event * e);
			void Update();

			friend class duckhacker::Game;

		private:
			bool button_states_[1];
			bool button_states_last_[1];

			int mouse_x_last_, mouse_y_last_;
			bool mouse_left_down_last_;

			int horizontal_, vertical_;

			int mouse_scroll_x_, mouse_scroll_y_;
		};
	}
}

#endif