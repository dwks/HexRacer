#ifndef PROJECT_INPUT__INPUT_KEY_ACTION_H
#define PROJECT_INPUT__INPUT_KEY_ACTION_H

#include "InputAction.h"

namespace Project {
namespace Input {

class InputKeyAction
	: public InputAction {
public:
	int key;
	bool invert;

	InputKeyAction(int _key, bool _invert, double off_value = 0.0, double on_value = 1.0)
		: key(_key), invert(_invert), InputAction(off_value, on_value) {}

	InputType type() const { return KEY; }
};

}  // namespace Input
}  // namespace Project

#endif
