#include <cstddef>  // for std::size_t
#include "InputManager.h"

#include "event/EventSystem.h"
#include "event/PlayerAction.h"
#include "event/CameraMovement.h"
#include "event/SetDebugDrawing.h"
#include "event/QuitEvent.h"
#include "event/TogglePainting.h"
#include "event/SetDebugCamera.h"
#include "event/PaintEvent.h"
#include "event/PauseGame.h"

#include "settings/SettingsManager.h"
#include "timing/AccelControl.h"

namespace Project {
namespace SDL {

InputManager::InputManager(int ms, ClientData *clientData)
    : TimedSubsystem(ms), clientData(clientData) {
    
    for(std::size_t x = 0; x < sizeof keyDown / sizeof *keyDown; x ++) {
        keyDown[x] = false;
    }

	inputMapper = new Input::InputMapper();
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
}

InputManager::~InputManager() {
    delete joystick;
	delete inputMapper;
}

void InputManager::init() {
    joystick = new JoystickManager();
    joystick->open();
}

void InputManager::handleEvent(SDL_Event *event) {

	inputMapper->handleEvent(event);

    switch(event->type) {
    case SDL_KEYDOWN:
        keyDown[event->key.keysym.sym] = true;
        
        break;
    case SDL_KEYUP:
        keyDown[event->key.keysym.sym] = false;
        
        break;
    }
}

void InputManager::doAction(unsigned long currentTime) {

	inputMapper->update(Input::INPUT_A_TURN);
	inputMapper->update(Input::INPUT_A_ACCELERATE);
	inputMapper->update(Input::INPUT_D_JUMP);
	inputMapper->update(Input::INPUT_D_RESET);
	inputMapper->update(Input::INPUT_D_PAINT);
	inputMapper->update(Input::INPUT_D_ERASE);

	if (inputMapper->getAnalogStatus(Input::INPUT_A_TURN) != 0.0) {
		EMIT_EVENT(new Event::PlayerAction(
			Event::PlayerAction::TURN,
			inputMapper->getAnalogStatus(Input::INPUT_A_TURN)));
	}

	if (inputMapper->getAnalogStatus(Input::INPUT_A_ACCELERATE) != 0.0) {
		EMIT_EVENT(new Event::PlayerAction(
				Event::PlayerAction::ACCELERATE,
				inputMapper->getAnalogStatus(Input::INPUT_A_ACCELERATE)));
	}

	if (inputMapper->getDigitalStatus(Input::INPUT_D_JUMP))
		EMIT_EVENT(new Event::PlayerAction(Event::PlayerAction::JUMP, 0.0));

	if (inputMapper->getDigitalStatus(Input::INPUT_D_RESET))
		EMIT_EVENT(new Event::PlayerAction(
            Event::PlayerAction::FIX_OFF_TRACK, 0.0));

	/*
    if(keyDown[SDLK_LEFT]) {
        EMIT_EVENT(new Event::PlayerAction(Event::PlayerAction::TURN, -1.0));
    }
    if(keyDown[SDLK_RIGHT]) {
        EMIT_EVENT(new Event::PlayerAction(Event::PlayerAction::TURN, +1.0));
    }
	
    if(keyDown[SDLK_UP]) {
        EMIT_EVENT(new Event::PlayerAction(
            Event::PlayerAction::ACCELERATE, +1.0));
    }
    if(keyDown[SDLK_DOWN]) {
        EMIT_EVENT(new Event::PlayerAction(
            Event::PlayerAction::ACCELERATE, -1.0));
    }
    if(keyDown[SDLK_SPACE]) {
        EMIT_EVENT(new Event::PlayerAction(Event::PlayerAction::JUMP, 0.0));
    }
	
    if(keyDown[SDLK_h]) {
        keyDown[SDLK_h] = false;
        EMIT_EVENT(new Event::PlayerAction(
            Event::PlayerAction::FIX_OFF_TRACK, 0.0));
    }
	*/
    
    handlePaint();
    //handleJoystick();
}

void InputManager::doPausedChecks() {
    /*if(keyDown[SDLK_ESCAPE]) {
        keyDown[SDLK_ESCAPE] = false;
        
        bool pause = Timing::AccelControl::getInstance()->getPaused();
        pause = !pause;
        
        LOG2(SDL, INPUT, (pause ? "Game paused" : "Game unpaused"));
        EMIT_EVENT(new Event::PauseGame(pause));
    }*/

	inputMapper->update(Input::INPUT_D_RELOAD_CONFIG);
	inputMapper->update(Input::INPUT_D_PHYSICS_DEBUG);
	inputMapper->update(Input::INPUT_D_PATH_DEBUG);
	inputMapper->update(Input::INPUT_D_CAMERA_DEBUG);
	inputMapper->update(Input::INPUT_A_CAMERA_X_SPEED);
	inputMapper->update(Input::INPUT_A_CAMERA_Z_SPEED);

	if (inputMapper->getDigitalTriggered(Input::INPUT_D_RELOAD_CONFIG)) {
        
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
	if(inputMapper->getDigitalTriggered(Input::INPUT_D_CAMERA_DEBUG)) {
        
        static bool debug = false;
        debug = !debug;
        EMIT_EVENT(new Event::SetDebugCamera(debug));
    }
    
    static const double CAMERA_FACTOR = 0.8;

	if (inputMapper->getAnalogStatus(Input::INPUT_A_CAMERA_X_SPEED) != 0.0 ||
		inputMapper->getAnalogStatus(Input::INPUT_A_CAMERA_Z_SPEED) != 0.0) {

		EMIT_EVENT(new Event::CameraMovement(CAMERA_FACTOR
				* Math::Point(
				inputMapper->getAnalogStatus(Input::INPUT_A_CAMERA_X_SPEED),
				inputMapper->getAnalogStatus(Input::INPUT_A_CAMERA_Z_SPEED))));
	}
}

void InputManager::handlePaint() {
    static int paint = 0;
    static bool painting = false;
    static bool erasing = false;
    
	if(inputMapper->getDigitalTriggered(Input::INPUT_D_PAINT)) {
        
        painting = !painting;
        erasing = false;
        paint = (paint + 1) % 8;
        
        if(painting) {
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
        
        if(erasing) {
            EMIT_EVENT(new Event::TogglePainting(clientData->getPlayerID(),
                Event::TogglePainting::ERASING));
        }
        else {
            EMIT_EVENT(new Event::TogglePainting(clientData->getPlayerID(),
                Event::TogglePainting::NOTHING));
        }
    }
}

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


}  // namespace SDL
}  // namespace Project
