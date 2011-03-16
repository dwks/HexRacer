#include "GlobalInputManager.h"
#include <vector>

namespace Project {
namespace Input {

	GlobalInputManager *GlobalInputManager::instance = NULL;

	GlobalInputManager::GlobalInputManager() {
		instance = this;
		joystick = new JoystickManager();
		inputMapper = new InputMapper();

		setInputMappings(XB360_WINDOWS);
	}

	GlobalInputManager::~GlobalInputManager() {
		delete joystick;
		delete inputMapper;
	}


	bool GlobalInputManager::findJoystick() {

		int joysticks = SDL_NumJoysticks();

		if (joysticks == 0)
			return false;

		for (int i = 0; i < joysticks; i++) {
			joystick->open(i);
			int buttons = joystick->getNumButtons();
			for (int j = 0; j < buttons; j++) {
				if (joystick->getButtonDown(j))
					return true;
			}
			joystick->close();
		}

		return false;
	}

	void GlobalInputManager::setInputMappings(PresetMapping mapping) {

		inputMapper->clearAllMappings();

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

		switch (mapping) {

			case XB360_WINDOWS:

				//Left-stick X - Turning
				inputMapper->addAxisToAnalogMapping(0, joystick, -1.0, 1.0, stick_deadzone,
					Input::INPUT_A_TURN, -1.0, 1.0);
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
