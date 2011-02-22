#include <algorithm>

#include "InputMapper.h"
#include "InputKeyAction.h"
#include "InputButtonAction.h"
#include "InputAxisAction.h"

#include "misc/DeleteFunctor.h"
#include "math/Values.h"

namespace Project {
namespace Input {

	InputMapper::InputMapper() {
		for(int i = 0; i < SDLK_LAST; i++) {
			keyDown[i] = false;
        }
	}

	InputMapper::~InputMapper() {
		clearAllMappings();
	}

	void InputMapper::handleEvent(SDL_Event *event) {
		switch(event->type) {
		case SDL_KEYDOWN:
			keyDown[event->key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			keyDown[event->key.keysym.sym] = false;
			break;
		}
	}

	void InputMapper::addKeyToDigitalMapping(int key, bool invert, DigitalInputEvent event_id) {
        digitalMappings[static_cast<int>(event_id)].push_back(
            new InputKeyAction(key, invert));
	}

	void InputMapper::addKeyToAnalogMapping(int key, bool invert, AnalogInputEvent event_id,
		double off_value, double on_value) {
        
        analogMappings[static_cast<int>(event_id)].push_back(
            new InputKeyAction(key, invert, off_value, on_value));
	}

	void InputMapper::addButtonToDigitalMapping(int button, SDL::JoystickManager* joystick,
		bool invert, DigitalInputEvent event_id) {
		digitalMappings[static_cast<int>(event_id)].push_back(new InputButtonAction(button, joystick, invert));
	}

	void InputMapper::addButtonToAnalogMapping(int button, SDL::JoystickManager* joystick, bool invert, AnalogInputEvent event_id,
		double off_value, double on_value) {
		analogMappings[static_cast<int>(event_id)].push_back(
			new InputButtonAction(button, joystick, invert, off_value, on_value)
			);
	}

	void InputMapper::addAxisToDigitalMapping(int axis, SDL::JoystickManager* joystick,
		double start_value, double end_value, double deadzone, DigitalInputEvent event_id) {

		digitalMappings[static_cast<int>(event_id)].push_back(
			new InputAxisAction(axis, joystick, start_value, end_value, deadzone)
			);
	}

	void InputMapper::addAxisToAnalogMapping(int axis, SDL::JoystickManager* joystick,
		double start_value, double end_value, double deadzone, AnalogInputEvent event_id,
		double off_value, double on_value) {

		analogMappings[static_cast<int>(event_id)].push_back(
			new InputAxisAction(axis, joystick, start_value, end_value, deadzone, off_value, on_value)
			);

	}

	void InputMapper::updateAll() {

		for (int i = 0; i < NUM_DIGITAL_TYPES; i++) {
			update(static_cast<DigitalInputEvent>(i));
		}

		for (int i = 0; i < NUM_ANALOG_TYPES; i++) {
			update(static_cast<AnalogInputEvent>(i));
		}
	}

	void InputMapper::update(DigitalInputEvent event_id) {
        int i = static_cast<int>(event_id);
        
		bool status = false;
		for (unsigned int j = 0; j < digitalMappings[i].size(); j++) {
			status = (status || inputActionToBool(digitalMappings[i][j]));
		}
        
        bool old_status = snapshot.getDigitalStatus(event_id);
        bool triggered = (!old_status && status);
        
        snapshot.setDigitalTriggered(event_id, triggered);
        snapshot.setDigitalStatus(event_id, status);
	}

	void InputMapper::update(AnalogInputEvent event_id) {
		int i = static_cast<int>(event_id);

		double status = 0.0;
		for (unsigned int j = 0; j < analogMappings[i].size(); j++) {
			status += inputActionToDouble(analogMappings[i][j]);
		}
		
		snapshot.setAnalogStatus(event_id, status);

	}

	void InputMapper::clearAllMappings() {

		for (int i = 0; i < NUM_DIGITAL_TYPES; i++) {
			clearMappings(static_cast<DigitalInputEvent>(i));
		}

		for (int i = 0; i < NUM_ANALOG_TYPES; i++) {
			clearMappings(static_cast<AnalogInputEvent>(i));
	
		}

	}

	void InputMapper::clearMappings(DigitalInputEvent event_id) {
		int index = static_cast<int>(event_id);
		for (unsigned int i = 0; i < digitalMappings[index].size(); i++) {
			delete digitalMappings[index][i];
		}
		digitalMappings[index].clear();
	}

	void InputMapper::clearMappings(AnalogInputEvent event_id) {
		int index = static_cast<int>(event_id);
		for (unsigned int i = 0; i < analogMappings[index].size(); i++) {
			delete analogMappings[index][i];
		}
		analogMappings[index].clear();
	}

	bool InputMapper::inputActionToBool(InputAction* action) {

		switch (action->type()) {

			case InputAction::KEY: {
				InputKeyAction* key_action = (InputKeyAction*) action;
				return (keyDown[key_action->key] ^ key_action->invert);
			}

			case InputAction::BUTTON: {
				InputButtonAction* button_action = (InputButtonAction*) action;
				return button_action->getState();
		   }

			case InputAction::AXIS: {
				InputAxisAction* axis_action = (InputAxisAction*) action;

				double value = axis_action->getStickValue();
				return (
					value < Math::maximum(axis_action->startValue, axis_action->endValue) &&
					value > Math::minimum(axis_action->startValue, axis_action->endValue)
					);
			}

			default: return false;
		}

	}

	double InputMapper::inputActionToDouble(InputAction* action) {

		switch (action->type()) {

			case InputAction::KEY: {

				InputKeyAction* key_action = (InputKeyAction*) action;
				if (keyDown[key_action->key] ^ key_action->invert)
					return action->onValue;
				else
					return action->offValue;
			}

			case InputAction::BUTTON: {

				InputButtonAction* button_action = (InputButtonAction*) action;
				if (button_action->getState())
					return action->onValue;
				else
					return action->offValue;
			}

			case InputAction::AXIS: {
				InputAxisAction* axis_action = (InputAxisAction*) action;
				return axis_action->getState();
			}

		   default: return 0.0;
		}

	}

}  // namespace Input
}  // namespace Project
