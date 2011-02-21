#ifndef PROJECT_INPUT__INPUT_ACTION_H
#define PROJECT_INPUT__INPUT_ACTION_H

namespace Project {
namespace Input {

class InputAction {
public:

	enum InputType {KEY, BUTTON, AXIS};

	double offValue;
	double onValue;

	InputAction(double off_value, double on_value)
		: offValue(off_value), onValue(on_value) {}

	virtual InputType type() const = 0;
};

}  // namespace Input
}  // namespace Project

#endif
