#include "InputAxisAction.h"
#include "math/Values.h"

namespace Project {
namespace Input {

	double InputAxisAction::getState() {
		double value = Math::bound(
			joystick->getNormalizedAxisValue(axis, deadzone),
			Math::minimum(startValue, endValue),
			Math::maximum(startValue, endValue)
			);
		return (value-startValue)/(endValue-startValue)*(onValue-offValue)+offValue;
	}

}  // namespace Input
}  // namespace Project
