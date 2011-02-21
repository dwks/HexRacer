#include <algorithm>

#include "InputMapper.h"
#include "InputKeyAction.h"
#include "InputButtonAction.h"

#include "misc/DeleteFunctor.h"

namespace Project {
namespace Input {

	InputMapper::InputMapper() {
		for (int i = 0; i < SDLK_LAST; i++)
			keyDown[i] = false;
	}

    InputMapper::~InputMapper() {
        for(int x = 0; x < NUM_DIGITAL_TYPES; x ++) {
            // we could use this instead!
            /*std::for_each(
                digitalMappings[x].begin(),
                digitalMappings[x].end(),
                Misc::DeleteFunctor<InputAction>());*/
            
            for(std::vector<InputAction*>::size_type y = 0;
                y < digitalMappings[x].size(); ++y) {
                
                delete digitalMappings[x][y];
            }
        }
        
        for(int x = 0; x < NUM_ANALOG_TYPES; x ++) {
            for(std::vector<InputAction*>::size_type y = 0;
                y < analogMappings[x].size(); ++y) {
                
                delete analogMappings[x][y];
            }
        }
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

	void InputMapper::update() {

		for (int i = 0; i < NUM_DIGITAL_TYPES; i++) {
			update(static_cast<DigitalInputEvent>(i));
		}

		for (int i = 0; i < NUM_ANALOG_TYPES; i++) {
			update(static_cast<AnalogInputEvent>(i));
	
		}
	}

	void InputMapper::update(DigitalInputEvent event_id) {

		int i = static_cast<int>(event_id);

		bool last_status = digitalEventStatus[i];

		digitalEventStatus[i] = false;
		for (unsigned int j = 0; j < digitalMappings[i].size(); j++) {
			digitalEventStatus[i] = (digitalEventStatus[i] || inputActionToBool(digitalMappings[i][j]));
		}

		digitalEventTriggered[i] = ((!last_status) && digitalEventStatus[i]);

	}

	void InputMapper::update(AnalogInputEvent event_id) {

		int i = static_cast<int>(event_id);

		analogEventStatus[i] = 0.0;
		for (unsigned int j = 0; j < analogMappings[i].size(); j++) {
			analogEventStatus[i] += inputActionToDouble(analogMappings[i][j]);
		}
	}

	bool InputMapper::inputActionToBool(InputAction* action) {

		switch (action->type()) {

			case InputAction::KEY: {
				InputKeyAction* key_action = (InputKeyAction*) action;
				return (keyDown[key_action->key] ^ key_action->invert);
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

		   default: return 0.0;
		}

	}

}  // namespace Input
}  // namespace Project
