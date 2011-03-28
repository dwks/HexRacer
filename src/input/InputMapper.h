#ifndef PROJECT_INPUT__INPUT_MAPPER_H
#define PROJECT_INPUT__INPUT_MAPPER_H

#include "InputAction.h"
#include "InputSnapshot.h"

#include "SDL_events.h"
#include "SDL_keysym.h"

#include "JoystickManager.h"

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
    
	/** Map a key to a digital input event
		@param key The internal SDL index of the key
		@param invert True if the key should be up to register as being pressed
		@param event_id The id of the event
	*/
	void addKeyToDigitalMapping(int key, bool invert, DigitalInputEvent event_id);

	/** Map a key to an analog input event
		@param key The internal SDL index of the key
		@param invert True if the key should be up to register as being pressed
		@param event_id The id of the event
		@param off_value The value sent to the analog state when the key is not pressed
		@param on_value The value sent to the analog state when the key is pressed
	*/
	void addKeyToAnalogMapping(int key, bool invert, AnalogInputEvent event_id,
		double off_value = 0.0, double on_value = 1.0);
    
	/** Map a joystick button to a digital input event
		@param button The index of the button
		@param joystick The joystick manager of the desired joystick
		@param invert True if the button should be up to register as being pressed
		@param event_id The id of the event
	*/
	void addButtonToDigitalMapping(int button, JoystickManager* joystick, bool invert, DigitalInputEvent event_id);
    
	/** Map a joystick button to an analog input event
		@param button The index of the button
		@param joystick The joystick manager of the desired joystick
		@param invert True if the button should be up to register as being pressed
		@param event_id The id of the event
		@param off_value The value sent to the analog state when the button is not pressed
		@param on_value The value sent to the analog state when the button is pressed
	*/
	void addButtonToAnalogMapping(int button, JoystickManager* joystick, bool invert, AnalogInputEvent event_id,
		double off_value = 0.0, double on_value = 1.0);

	/** Map a joystick axis to a digital input event (Event registers as true when axis is within a range)
		@param axis The index of the axis
		@param joystick The joystick manager of the desired joystick
		@param start_value The lower-bound of the trigger range
		@param end_value The upper-bound of the trigger range
		@param deadzone The deadzone of the axis
		@param event_id The id of the event
	*/
	void addAxisToDigitalMapping(int axis, JoystickManager* joystick,
		double start_value, double end_value, double deadzone, DigitalInputEvent event_id);

	/** Map a joystick axis to a analog input event
		@param axis The index of the axis
		@param joystick The joystick manager of the desired joystick
		@param start_value The axis-position corresponding to 0%
		@param end_value The axis-position corresponding to 100%
		@param deadzone The deadzone of the axis
		@param event_id The id of the event
		@param off_value The value sent to the analog state corresponding to 0%
		@param on_value The value sent to the analog state corresponding to 100%
	*/
	void addAxisToAnalogMapping(int axis, JoystickManager* joystick,
		double start_value, double end_value, double deadzone, AnalogInputEvent event_id,
		double off_value, double on_value);

	void clearAllMappings();
	void clearMappings(DigitalInputEvent event_id);
	void clearMappings(AnalogInputEvent event_id);

	void updateAll();
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
