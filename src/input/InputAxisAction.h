#ifndef PROJECT_INPUT__INPUT_AXIS_ACTION_H
#define PROJECT_INPUT__INPUT_AXIS_ACTION_H

#include "JoystickManager.h"
#include "InputAction.h"

namespace Project {
namespace Input {

class InputAxisAction
	: public InputAction {
public:
	int axis;
	JoystickManager* joystick;
	double startValue;
	double endValue;
	double deadzone;

	InputAxisAction(int _axis, JoystickManager* _joystick,
		double start_value, double end_value, double _deadzone = 0.0,
		double off_value = 0.0, double on_value = 1.0)
		: InputAction(off_value, on_value), axis(_axis), joystick(_joystick),
		startValue(start_value), endValue(end_value), deadzone(_deadzone) {}

	double getStickValue() {
		return joystick->getNormalizedAxisValue(axis, deadzone);
	}

	double getState();

	InputType type() const { return AXIS; }
};

}  // namespace Input
}  // namespace Project

#endif
