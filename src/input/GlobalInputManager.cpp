#include "GlobalInputManager.h"
#include <vector>

namespace Project {
namespace Input {

	GlobalInputManager *GlobalInputManager::instance = NULL;

	GlobalInputManager::GlobalInputManager() {
		instance = this;
		joystick = new JoystickManager();
		//openFirstJoystick();
		inputMapper = new InputMapper();

		setInputMappings(XB360_WINDOWS);
	}

	GlobalInputManager::~GlobalInputManager() {
		delete joystick;
		delete inputMapper;
	}

    void GlobalInputManager::startFindJoystick() {
        allJoysticks.clear();
        
        int joysticks = SDL_NumJoysticks();
        for(int j = 0; j < joysticks; j ++) {
            JoystickManager *joy = new JoystickManager();
            joy->open(j);
            allJoysticks.push_back(joy);
        }
    }

	bool GlobalInputManager::findJoystick() {
		int joysticks = static_cast<int>(allJoysticks.size());

		if (joysticks == 0)
			return false;

		for (int j = 0; j < joysticks; j++) {
            JoystickManager *joy = allJoysticks[j];
            if(!joy->hasJoystick()) continue;
            
            /*for(int axis = 0; axis < joystick->getNumAxes(); axis ++) {
                if(joystick->getNormalizedAxisValue(axis, 0.5)) {
                    return true;
                }
            }*/
            
            int buttons = joystick->getNumButtons();
            for (int b = 0; b < buttons; b++) {
                if (joy->getButtonDown(b)) {
                    cancelFindJoystick();
                    
                    joystick->close();
                    joystick->open(j);
                    
                    return true;
                }
            }
		}

		return false;
	}
    
    void GlobalInputManager::cancelFindJoystick(int exceptFor) {
        int joysticks = static_cast<int>(allJoysticks.size());
        for(int j = 0; j < joysticks; j ++) {
            if(j == exceptFor) continue;
            
            delete allJoysticks[j];
        }
        allJoysticks.clear();
    }
    
	bool GlobalInputManager::openFirstJoystick() {

		joystick->close();

		int joysticks = SDL_NumJoysticks();

		if (joysticks == 0)
			return false;
		else
			return joystick->open(0);

	}

	void GlobalInputManager::setInputMappings(PresetMapping mapping) {

		inputMapper->clearAllMappings();

		//Menu mappings
		inputMapper->addKeyToDigitalMapping(SDLK_LEFT, false, Input::INPUT_D_MENU_LEFT);
		inputMapper->addKeyToDigitalMapping(SDLK_RIGHT, false, Input::INPUT_D_MENU_RIGHT);
		inputMapper->addKeyToDigitalMapping(SDLK_UP, false, Input::INPUT_D_MENU_UP);
		inputMapper->addKeyToDigitalMapping(SDLK_DOWN, false, Input::INPUT_D_MENU_DOWN);
		inputMapper->addKeyToDigitalMapping(SDLK_RETURN, false, Input::INPUT_D_MENU_CONFIRM);
		inputMapper->addKeyToDigitalMapping(SDLK_BACKSPACE, false, Input::INPUT_D_MENU_BACK);

		//Standard keyboard mappings
		inputMapper->addKeyToAnalogMapping(SDLK_LEFT, false, Input::INPUT_A_TURN, 0.0, -1.0);
		inputMapper->addKeyToAnalogMapping(SDLK_RIGHT, false, Input::INPUT_A_TURN, 0.0, 1.0);

		inputMapper->addKeyToAnalogMapping(SDLK_UP, false, Input::INPUT_A_ACCELERATE, 0.0, 1.0);
		inputMapper->addKeyToAnalogMapping(SDLK_DOWN, false, Input::INPUT_A_ACCELERATE, 0.0, -1.0);

		inputMapper->addKeyToAnalogMapping(SDLK_w, false, Input::INPUT_A_CAMERA_Z_SPEED, 0.0, 1.0);
		inputMapper->addKeyToAnalogMapping(SDLK_s, false, Input::INPUT_A_CAMERA_Z_SPEED, 0.0, -1.0);

		inputMapper->addKeyToAnalogMapping(SDLK_a, false, Input::INPUT_A_CAMERA_X_SPEED, 0.0, -1.0);
		inputMapper->addKeyToAnalogMapping(SDLK_d, false, Input::INPUT_A_CAMERA_X_SPEED, 0.0, 1.0);

		inputMapper->addKeyToDigitalMapping(SDLK_SPACE, false, Input::INPUT_D_JUMP);
		inputMapper->addKeyToDigitalMapping(SDLK_h, false, Input::INPUT_D_WARP);
		inputMapper->addKeyToDigitalMapping(SDLK_RETURN, false, Input::INPUT_D_PHYSICS_DEBUG);
		inputMapper->addKeyToDigitalMapping(SDLK_BACKSLASH, false, Input::INPUT_D_PATH_DEBUG);
		inputMapper->addKeyToDigitalMapping(SDLK_c, false, Input::INPUT_D_CAMERA_DEBUG);
		inputMapper->addKeyToDigitalMapping(SDLK_F5, false, Input::INPUT_D_RELOAD_CONFIG);

		inputMapper->addKeyToDigitalMapping(SDLK_p, false, Input::INPUT_D_PAINT);
		inputMapper->addKeyToDigitalMapping(SDLK_o, false, Input::INPUT_D_ERASE);
		inputMapper->addKeyToDigitalMapping(SDLK_z, false, Input::INPUT_D_PAINT);
		inputMapper->addKeyToDigitalMapping(SDLK_x, false, Input::INPUT_D_ERASE);

		//Joystick mappings

		if (!joystick->hasJoystick())
			return;

		const double stick_deadzone = 0.15;

		const double axis_menu_thresh = 0.6;

		switch (mapping) {

			case XB360_WINDOWS:

				//Left-stick X - Turning
				inputMapper->addAxisToAnalogMapping(0, joystick, -1.0, 1.0, stick_deadzone,
					Input::INPUT_A_TURN, -1.0, 1.0);

				inputMapper->addAxisToDigitalMapping(0, joystick, axis_menu_thresh, 2.0, stick_deadzone,
					Input::INPUT_D_MENU_RIGHT);
				inputMapper->addAxisToDigitalMapping(0, joystick, -axis_menu_thresh, -2.0, stick_deadzone,
					Input::INPUT_D_MENU_LEFT);
				inputMapper->addAxisToDigitalMapping(1, joystick, axis_menu_thresh, 2.0, stick_deadzone,
					Input::INPUT_D_MENU_DOWN);
				inputMapper->addAxisToDigitalMapping(1, joystick, -axis_menu_thresh, -2.0, stick_deadzone,
					Input::INPUT_D_MENU_UP);

				//Right/Left Trigger Axis - Accelerate/Braking
				inputMapper->addAxisToAnalogMapping(2, joystick, 1.0, -1.0, stick_deadzone,
					Input::INPUT_A_ACCELERATE, -1.0, 1.0);

				//A - Paint
				inputMapper->addButtonToDigitalMapping(0, joystick, false, Input::INPUT_D_PAINT);
				//B - Erase
				inputMapper->addButtonToDigitalMapping(1, joystick, false, Input::INPUT_D_ERASE);
				//Start - Pause
				inputMapper->addButtonToDigitalMapping(7, joystick, false, Input::INPUT_D_TOGGLE_PAUSED);

				/*
				//Left-stick X - Debug camera X
				inputMapper->addAxisToAnalogMapping(0, joystick, -1.0, 1.0, stick_deadzone,
					Input::INPUT_A_CAMERA_X_SPEED, -1.0, 1.0);
				//Left-stick Y - Debug camera Z
				inputMapper->addAxisToAnalogMapping(1, joystick, 1.0, -1.0, stick_deadzone,
					Input::INPUT_A_CAMERA_Z_SPEED, -1.0, 1.0);
				//Select - Debug camera
				inputMapper->addButtonToDigitalMapping(6, joystick, false, Input::INPUT_D_CAMERA_DEBUG);
				*/

				break;

			default: break;
		}

	}

}  // namespace Input
}  // namespace Project
