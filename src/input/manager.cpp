#include "input/manager.hpp"

#include <cstring>

#include "external/imgui/imgui.h"

namespace duckhacker
{
	namespace input
	{
		Manager::Manager()
		{
			memset(button_states_, 0, sizeof(button_states_));
			memset(button_states_last_, 0, sizeof(button_states_last_));

			horizontal_ = 0;
			vertical_ = 0;

			mouse_x_last_ = 0;
			mouse_y_last_ = 0;
			mouse_left_down_last_ = false;

			mouse_scroll_x_ = 0;
			mouse_scroll_y_ = 0;
		}

		double Manager::GetAxis(Axis axis)
		{
			if (axis == Axis::Horizontal)
			{
				return horizontal_;
			}
			else if (axis == Axis::Vertical)
			{
				return vertical_;
			}
			else if (axis == Axis::MouseScrollX)
			{
				return mouse_scroll_x_;
			}
			else if (axis == Axis::MouseScrollY)
			{
				return mouse_scroll_y_;
			}
			return 0;
		}

		bool Manager::WasButtonPressed(Button button)
		{
			return button_states_[button] && !button_states_last_[button];
		}

		void Manager::EarlyUpdate()
		{
			mouse_scroll_x_ = 0;
			mouse_scroll_y_ = 0;

			memcpy(button_states_last_, button_states_, sizeof(button_states_));
		}

		void Manager::ProcessEvent(SDL_Event * e)
		{
			ImGuiIO& io = ImGui::GetIO(); (void)io;

			if (e->type == SDL_MOUSEWHEEL && !io.WantCaptureMouse)
			{
				if (e->wheel.x != 0)
				{
					mouse_scroll_x_ = e->wheel.x;
				}
				if (e->wheel.y != 0)
				{
					mouse_scroll_y_ = e->wheel.y;
				}
			}
		}

		void Manager::Update()
		{
			ImGuiIO& io = ImGui::GetIO(); (void)io;

			if (!io.WantCaptureMouse)
			{
				int mouse_x, mouse_y;
				uint32_t mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
				bool mouse_left_down = ((mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0);

				if (mouse_left_down && mouse_left_down_last_)
				{
					horizontal_ = (mouse_x - mouse_x_last_);
					vertical_ = (mouse_y - mouse_y_last_);
				}
				else
				{
					horizontal_ = 0;
					vertical_ = 0;
				}

				mouse_x_last_ = mouse_x;
				mouse_y_last_ = mouse_y;
				mouse_left_down_last_ = mouse_left_down;
			}
			else
			{
				horizontal_ = 0;
				vertical_ = 0;

				mouse_scroll_x_ = 0;
				mouse_scroll_y_ = 0;
			}

			if (!io.WantCaptureKeyboard)
			{
				const uint8_t * keyboard_state = SDL_GetKeyboardState(nullptr);

				button_states_[Continue] = keyboard_state[SDL_SCANCODE_SPACE];
			}
		}
	}
}