#ifndef PROJECT_INPUT__INPUT_BUTTON_ACTION_H
#define PROJECT_INPUT__INPUT_BUTTON_ACTION_H

#include "sdl/JoystickManager.h"
#include "InputAction.h"

namespace Project {
namespace Input {

class InputButtonAction
	: public InputAction {
public:
	int button;
	bool invert;
	SDL::JoystickManager* joystick;

	InputButtonAction(int _button, SDL::JoystickManager* _joystick, bool _invert,
		double off_value = 0.0, double on_value = 1.0)
		: InputAction(off_value, on_value), button(_button), joystick(_joystick), invert(_invert) {}

	bool getState() { return (joystick->getButtonDown(button)^invert); }

	InputType type() const { return BUTTON; }
};

}  // namespace Input
}  // namespace Project

#endif
