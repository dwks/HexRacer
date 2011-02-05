#include <cstddef>  // for std::size_t
#include "InputManager.h"

#include "event/ObserverList.h"
#include "event/PlayerAction.h"
#include "event/CameraMovement.h"
#include "event/SetDebugDrawing.h"
#include "event/QuitEvent.h"
#include "event/TogglePainting.h"
#include "event/SetDebugCamera.h"
#include "event/PaintEvent.h"
#include "event/PauseGame.h"

#include "settings/SettingsManager.h"

namespace Project {
namespace SDL {

InputManager::InputManager(int ms, ClientData *clientData)
    : TimedSubsystem(ms), clientData(clientData) {
    
    for(std::size_t x = 0; x < sizeof keyDown / sizeof *keyDown; x ++) {
        keyDown[x] = false;
    }
}

InputManager::~InputManager() {
    delete joystick;
}

void InputManager::init() {
    joystick = new JoystickManager();
    joystick->open();
}

void InputManager::handleEvent(SDL_Event *event) {
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
        EMIT_EVENT(new Event::PlayerAction(
            Event::PlayerAction::FIX_OFF_TRACK, 0.0));
    }
    
    handlePaint();
    handleJoystick();
}

void InputManager::doPausedChecks() {
    if(keyDown[SDLK_ESCAPE]) {
        EMIT_EVENT(new Event::QuitEvent());
    }
    if(keyDown[SDLK_F1]) {
        keyDown[SDLK_F1] = false;
        
        static bool pause = false;
        pause = !pause;
        
        LOG2(SDL, INPUT, (pause ? "Game paused" : "Game unpaused"));
        
        EMIT_EVENT(new Event::PauseGame(pause));
    }
    if(keyDown[SDLK_F5]) {
        keyDown[SDLK_F5] = false;
        
        LOG2(GLOBAL, SETTING,
            "Reloading config file \"" << CONFIG_FILE << "\"");
        
        Settings::SettingsManager::getInstance()->removeAll();
        Settings::SettingsManager::getInstance()->load(CONFIG_FILE);
    }
    if(keyDown[SDLK_RETURN]) {
        keyDown[SDLK_RETURN] = false;
        
        static bool debug = false;
        debug = !debug;
        EMIT_EVENT(new Event::SetDebugDrawing(debug));
    }
    if(keyDown[SDLK_c]) {
        keyDown[SDLK_c] = false;
        
        static bool debug = false;
        debug = !debug;
        EMIT_EVENT(new Event::SetDebugCamera(debug));
    }
    
    static const double CAMERA_FACTOR = 0.8;
    if(keyDown[SDLK_w]) {
        EMIT_EVENT(new Event::CameraMovement(CAMERA_FACTOR
            * Math::Point(0.0, 1.0)));
    }
    if(keyDown[SDLK_a]) {
        EMIT_EVENT(new Event::CameraMovement(CAMERA_FACTOR
            * Math::Point(-1.0, 0.0)));
    }
    if(keyDown[SDLK_s]) {
        EMIT_EVENT(new Event::CameraMovement(CAMERA_FACTOR
            * Math::Point(0.0, -1.0)));
    }
    if(keyDown[SDLK_d]) {
        EMIT_EVENT(new Event::CameraMovement(CAMERA_FACTOR
            * Math::Point(+1.0, 0.0)));
    }
}

void InputManager::handlePaint() {
    static int paint = 0;
    static bool painting = false;
    static bool erasing = false;
    
    if(keyDown[SDLK_p]) {
        keyDown[SDLK_p] = false;
        
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
    if(keyDown[SDLK_o]) {
        keyDown[SDLK_o] = false;
        
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
