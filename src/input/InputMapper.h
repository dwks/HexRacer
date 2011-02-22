#ifndef PROJECT_INPUT__INPUT_MAPPER_H
#define PROJECT_INPUT__INPUT_MAPPER_H

#include "InputAction.h"
#include "InputSnapshot.h"

#include "SDL_events.h"
#include "SDL_keysym.h"

#include "sdl/JoystickManager.h"

#include <vector>

namespace Project {
namespace Input {

class InputMapper {
private:

	bool keyDown[SDLK_LAST];
    InputSnapshot snapshot;

	std::vector<InputAction*> digitalMappings [NUM_DIGITAL_TYPES];
	std::vector<InputAction*> analogMappings [NUM_ANALOG_TYPES];

	bool inputActionToBool(InputAction* action);
	double inputActionToDouble(InputAction* action);

public:

	InputMapper();
    ~InputMapper();
    
	void handleEvent(SDL_Event *event);
    
	void addKeyToDigitalMapping(int key, bool invert, DigitalInputEvent event_id);
	void addKeyToAnalogMapping(int key, bool invert, AnalogInputEvent event_id,
		double off_value = 0.0, double on_value = 1.0);
	//void addButtonToDigitalMapping(int button, SDL::JoystickManager* joystick, bool invert, DigitalInputEvent event_id);
    
	void clearMappings();
    
	void update();
	void update(DigitalInputEvent event_id);
	void update(AnalogInputEvent event_id);
    
	bool getDigitalStatus(DigitalInputEvent type) const
        { return snapshot.getDigitalStatus(type); }
	bool getDigitalTriggered(DigitalInputEvent type) const
        { return snapshot.getDigitalTriggered(type); }
	double getAnalogStatus(AnalogInputEvent type) const
        { return snapshot.getAnalogStatus(type); }
    
    const InputSnapshot &getSnapshot() const { return snapshot; }
};

}  // namespace Input
}  // namespace Project

#endif
