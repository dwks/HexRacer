#ifndef PROJECT_INPUT__INPUT_BUTTON_ACTION_H
#define PROJECT_INPUT__INPUT_BUTTON_ACTION_H

#include "JoystickManager.h"
#include "InputAction.h"

namespace Project {
namespace Input {

class InputButtonAction
	: public InputAction {
public:
	int button;
	bool invert;
	JoystickManager* joystick;

	InputButtonAction(int _button, JoystickManager* _joystick, bool _invert,
		double off_value = 0.0, double on_value = 1.0)
		: InputAction(off_value, on_value), button(_button), invert(_invert), joystick(_joystick) {}

	bool getState() { return (joystick->getButtonDown(button)^invert); }

	InputType type() const { return BUTTON; }
};

}  // namespace Input
}  // namespace Project

#endif
