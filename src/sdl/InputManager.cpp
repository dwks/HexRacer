#include <cstddef>  // for std::size_t
#include "InputManager.h"

#include "event/ObserverList.h"
#include "event/PlayerAction.h"
#include "event/CameraMovement.h"
#include "event/SetDebugDrawing.h"
#include "event/QuitEvent.h"
#include "event/TogglePainting.h"

#include "event/PaintEvent.h"

#include "settings/SettingsManager.h"

namespace Project {
namespace SDL {

InputManager::InputManager(int ms, ClientData *clientData, PlayerManager *playerManager)
    : TimedSubsystem(ms), playerManager(playerManager), clientData(clientData) {
    
    for(std::size_t x = 0; x < sizeof keyDown / sizeof *keyDown; x ++) {
        keyDown[x] = false;
    }
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
    static const double CAMERA_FACTOR = 0.8;
    
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
    
    if(keyDown[SDLK_ESCAPE]) {
        EMIT_EVENT(new Event::QuitEvent());
    }
    
    if(keyDown[SDLK_h]) {
        EMIT_EVENT(new Event::PlayerAction(
            Event::PlayerAction::FIX_OFF_TRACK, 0.0));
    }
    
    if(keyDown[SDLK_RETURN]) {
        keyDown[SDLK_RETURN] = false;
        
        static bool debug = false;
        debug = !debug;
        EMIT_EVENT(new Event::SetDebugDrawing(debug));
    }
    
    {
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
        
        static double PAINTING_RADIUS = 1.5;
        
        if(GET_SETTING("render.paint.legacypaint", 0)) {
            if(painting) {
                EMIT_EVENT(new Event::PaintEvent(
                    playerManager->getPlayer()->getPosition(),
                    PAINTING_RADIUS,
                    paint));
            }
            if(erasing) {
                EMIT_EVENT(new Event::PaintEvent(
                    playerManager->getPlayer()->getPosition(),
                    PAINTING_RADIUS,
                    -1));
            }
        }
    }
}

}  // namespace SDL
}  // namespace Project
