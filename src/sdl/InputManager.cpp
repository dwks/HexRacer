#include <cstddef>  // for std::size_t
#include "InputManager.h"
#include "input/GlobalInputManager.h"

#include "event/EventSystem.h"
#include "event/ChangeOfIntention.h"
#include "event/CameraMovement.h"
#include "event/SetDebugDrawing.h"
#include "event/QuitEvent.h"
#include "event/TogglePainting.h"
#include "event/SetDebugCamera.h"
#include "event/PaintEvent.h"
#include "event/PauseGame.h"
#include "event/ModifyCamera.h"

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
    //delete joystick;
	//delete inputMapper;
}

void InputManager::init() {
	/*
    joystick = new JoystickManager();
    joystick->open();
	inputMapper = new Input::InputMapper();
	setInputMappings(XB360_WINDOWS);
	*/
	inputMapper = Input::GlobalInputManager::getInstance()->getInputMapper();
}

void InputManager::doAction(unsigned long currentTime) {
	inputMapper->update(Input::INPUT_A_TURN);
	inputMapper->update(Input::INPUT_A_ACCELERATE);
	inputMapper->update(Input::INPUT_D_JUMP);
	inputMapper->update(Input::INPUT_D_WARP);
    
    World::PlayerIntention intention
        = playerManager->getPlayer()->getIntention();
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
    inputMapper->update(Input::INPUT_D_CAMERA_SWITCH_FOCUS);
    inputMapper->update(Input::INPUT_D_CAMERA_SWITCH_FOCUS_INV);
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
    if (inputMapper->getDigitalTriggered(Input::INPUT_D_CAMERA_SWITCH_FOCUS)) {
        EMIT_EVENT(new Event::ModifyCamera(
            Event::ModifyCamera::FOCUS_PLAYER, 1.0));
    }
    if (inputMapper->getDigitalTriggered(Input::INPUT_D_CAMERA_SWITCH_FOCUS_INV)) {
        EMIT_EVENT(new Event::ModifyCamera(
            Event::ModifyCamera::FOCUS_PLAYER, -1.0));
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




}  // namespace SDL
}  // namespace Project
