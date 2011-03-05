#include <cstddef>  // for std::size_t
#include "InputManager.h"

#include "event/EventSystem.h"
#include "event/ChangeOfIntention.h"
#include "event/CameraMovement.h"
#include "event/SetDebugDrawing.h"
#include "event/QuitEvent.h"
#include "event/TogglePainting.h"
#include "event/SetDebugCamera.h"
#include "event/PaintEvent.h"
#include "event/PauseGame.h"

#include "settings/SettingsManager.h"
#include "timing/AccelControl.h"

#include "math/Values.h"

#include "log/Logger.h"
#include "config.h"

namespace Project {
namespace SDL {

InputManager::InputManager(int ms, ClientData *clientData,
    World::PlayerManager *playerManager)
    : TimedSubsystem(ms), clientData(clientData),
    playerManager(playerManager) {
    
	debugCamera = false;
}

InputManager::~InputManager() {
    delete joystick;
	delete inputMapper;
}

void InputManager::init() {
    joystick = new JoystickManager();
    joystick->open();

	inputMapper = new Input::InputMapper();

	setInputMappings(XB360_WINDOWS);
}

void InputManager::handleEvent(SDL_Event *event) {
	inputMapper->handleEvent(event);
}

void InputManager::doAction(unsigned long currentTime) {
	inputMapper->update(Input::INPUT_A_TURN);
	inputMapper->update(Input::INPUT_A_ACCELERATE);
	inputMapper->update(Input::INPUT_D_JUMP);
	inputMapper->update(Input::INPUT_D_RESET);
    
    World::PlayerIntention intention;
    inputMapper->getSnapshot().asPlayerIntention(intention);
    
    bool identical = (intention == playerManager->getPlayer()->getIntention());
    if(!identical) {
        playerManager->getPlayer()->setIntention(intention);
        
        EMIT_EVENT(new Event::ChangeOfIntention(
            clientData->getPlayerID(),
            intention));
    }
    
    handlePaint();
}

void InputManager::doPausedChecks() {
	inputMapper->update(Input::INPUT_D_RELOAD_CONFIG);
	inputMapper->update(Input::INPUT_D_PHYSICS_DEBUG);
	inputMapper->update(Input::INPUT_D_PATH_DEBUG);
	inputMapper->update(Input::INPUT_D_CAMERA_DEBUG);
	inputMapper->update(Input::INPUT_D_TOGGLE_PAUSED);
	inputMapper->update(Input::INPUT_A_CAMERA_X_SPEED);
	inputMapper->update(Input::INPUT_A_CAMERA_Z_SPEED);

	if(inputMapper->getDigitalTriggered(Input::INPUT_D_RELOAD_CONFIG)) {
        
        LOG2(GLOBAL, SETTING,
            "Reloading config file \"" << CONFIG_FILE << "\"");
        
        Settings::SettingsManager::getInstance()->removeAll();
        Settings::SettingsManager::getInstance()->load(CONFIG_FILE);
        Settings::SettingsManager::getInstance()->dump();
    }
    if(inputMapper->getDigitalTriggered(Input::INPUT_D_PHYSICS_DEBUG)) {
        
        static bool debug = false;
        debug = !debug;
        EMIT_EVENT(new Event::SetDebugDrawing(debug));
    }
	if(inputMapper->getDigitalTriggered(Input::INPUT_D_PATH_DEBUG)) {
        
        if(!GET_SETTING("render.drawpathnodes", 0)) {
            Settings::SettingsManager::getInstance()->set(
                "render.drawpathnodes", "1");
        }
        else {
            Settings::SettingsManager::getInstance()->set(
                "render.drawpathnodes", "0");
        }
    }
	if (inputMapper->getDigitalTriggered(Input::INPUT_D_CAMERA_DEBUG)) {
        
        //static bool debug = false;
        debugCamera = !debugCamera;
        EMIT_EVENT(new Event::SetDebugCamera(debugCamera));
    }

	if (inputMapper->getDigitalTriggered(Input::INPUT_D_TOGGLE_PAUSED)) {
		bool paused = Timing::AccelControl::getInstance()->getPaused();
		EMIT_EVENT(new Event::PauseGame(!paused));
	}

	if (debugCamera) {
    
		static const double CAMERA_FACTOR = 0.8;

		if (inputMapper->getAnalogStatus(Input::INPUT_A_CAMERA_X_SPEED) != 0.0 ||
			inputMapper->getAnalogStatus(Input::INPUT_A_CAMERA_Z_SPEED) != 0.0) {

			EMIT_EVENT(new Event::CameraMovement(CAMERA_FACTOR
					* Math::Point(
					inputMapper->getAnalogStatus(Input::INPUT_A_CAMERA_X_SPEED),
					inputMapper->getAnalogStatus(Input::INPUT_A_CAMERA_Z_SPEED))));
		}
	}
}

void InputManager::handlePaint() {

	inputMapper->update(Input::INPUT_D_PAINT);
	inputMapper->update(Input::INPUT_D_ERASE);

    static int paint = 0;
    static bool painting = false;
    static bool erasing = false;
    
	if(inputMapper->getDigitalTriggered(Input::INPUT_D_PAINT)) {
        
        painting = !painting;
        erasing = false;
        paint = (paint + 1) % 8;
        
        if (painting) {
            EMIT_EVENT(new Event::TogglePainting(clientData->getPlayerID(),
                Event::TogglePainting::PAINTING));
        }
        else {
            EMIT_EVENT(new Event::TogglePainting(clientData->getPlayerID(),
                Event::TogglePainting::NOTHING));
        }
    }
    if(inputMapper->getDigitalTriggered(Input::INPUT_D_ERASE)) {
        
        erasing = !erasing;
        painting = false;
        
        if (erasing) {
            EMIT_EVENT(new Event::TogglePainting(clientData->getPlayerID(),
                Event::TogglePainting::ERASING));
        }
        else {
            EMIT_EVENT(new Event::TogglePainting(clientData->getPlayerID(),
                Event::TogglePainting::NOTHING));
        }
    }
}

/*
void InputManager::handleJoystick() {
    const double DEADZONE = 0.2;
    
    // left hat
    double leftX = joystick->getNormalizedAxisValue(0, DEADZONE);
    //double leftY = joystick->getNormalizedAxisValue(1, DEADZONE);
    
    // right hat
    //double rightX = joystick->getNormalizedAxisValue(3, DEADZONE);
    double rightY = -joystick->getNormalizedAxisValue(2, DEADZONE);
    
    if(leftX) {
        EMIT_EVENT(new Event::PlayerAction(Event::PlayerAction::TURN, leftX));
    }
    
    if(rightY) {
        EMIT_EVENT(new Event::PlayerAction(
            Event::PlayerAction::ACCELERATE, rightY));
    }
}
*/

void InputManager::setInputMappings(PresetMapping mapping) {

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
	inputMapper->addKeyToDigitalMapping(SDLK_h, false, Input::INPUT_D_RESET);
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


}  // namespace SDL
}  // namespace Project
