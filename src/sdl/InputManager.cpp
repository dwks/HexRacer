#include <cstddef>  // for std::size_t
#include "InputManager.h"

#include "event/ObserverList.h"
#include "event/PlayerMovement.h"
#include "event/CameraMovement.h"

namespace Project {
namespace SDL {

InputManager::InputManager() {
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

void InputManager::advanceToNextFrame() {
    static const double MOVEMENT_FACTOR = 0.003;
    static const double CAMERA_FACTOR = 0.8;
    
    if(keyDown[SDLK_LEFT]) {
        EMIT_EVENT(new Event::PlayerMovement(MOVEMENT_FACTOR
            * Math::Point(+1.0, 0.0)));
    }
    if(keyDown[SDLK_RIGHT]) {
        EMIT_EVENT(new Event::PlayerMovement(MOVEMENT_FACTOR
            * Math::Point(-1.0, 0.0)));
    }
    if(keyDown[SDLK_UP]) {
        EMIT_EVENT(new Event::PlayerMovement(MOVEMENT_FACTOR
            * Math::Point(0.0, 0.0, +1.0)));
    }
    if(keyDown[SDLK_DOWN]) {
        EMIT_EVENT(new Event::PlayerMovement(MOVEMENT_FACTOR
            * Math::Point(0.0, 0.0, -1.0)));
    }
    if(keyDown[SDLK_SPACE]) {
        EMIT_EVENT(new Event::PlayerMovement(MOVEMENT_FACTOR
            * Math::Point(0.0, +3.0, 0.0)));
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
}

}  // namespace SDL
}  // namespace Project
