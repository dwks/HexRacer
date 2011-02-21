#ifndef PROJECT_INPUT__INPUT_MAPPER_H
#define PROJECT_INPUT__INPUT_MAPPER_H

#include "InputAction.h"
#include "InputEventTypes.h"

#include "SDL_events.h"
#include "SDL_keysym.h"

#include "sdl/JoystickManager.h"

#include <vector>

namespace Project {
namespace Input {

class InputMapper {
private:

	enum InputType {DIGITAL, ANALOG};

	bool keyDown[SDLK_LAST];
	bool digitalEventStatus[NUM_DIGITAL_TYPES];
	bool digitalEventTriggered[NUM_DIGITAL_TYPES];
	double analogEventStatus[NUM_ANALOG_TYPES];

	std::vector<InputAction*> digitalMappings [NUM_DIGITAL_TYPES];
	std::vector<InputAction*> analogMappings [NUM_ANALOG_TYPES];

	bool inputActionToBool(InputAction* action);
	double inputActionToDouble(InputAction* action);

public:

	InputMapper();

	void handleEvent(SDL_Event *event);

	void addKeyToDigitalMapping(int key, bool invert, DigitalInputEvent event_id);
	void addKeyToAnalogMapping(int key, bool invert, AnalogInputEvent event_id,
		double off_value = 0.0, double on_value = 1.0);
	//void addButtonToDigitalMapping(int button, SDL::JoystickManager* joystick, bool invert, DigitalInputEvent event_id);

	void clearMappings();

	void update();
	void update(DigitalInputEvent event_id);
	void update(AnalogInputEvent event_id);

	bool getDigitalStatus(DigitalInputEvent type) { return digitalEventStatus[static_cast<int>(type)]; }
	bool getDigitalTriggered(DigitalInputEvent type) { return digitalEventTriggered[static_cast<int>(type)]; }
	double getAnalogStatus(AnalogInputEvent type) { return analogEventStatus[static_cast<int>(type)]; }

};

}  // namespace Input
}  // namespace Project

#endif
