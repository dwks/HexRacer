#include <cstddef>  // for std::size_t
#include "InputManager.h"

#include "event/ObserverList.h"
#include "event/PlayerMovement.h"

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
    if(keyDown[SDLK_LEFT]) {
        EMIT_EVENT(new Event::PlayerMovement(Math::Point(-0.01, 0.0)));
    }
    if(keyDown[SDLK_RIGHT]) {
        EMIT_EVENT(new Event::PlayerMovement(Math::Point(+0.01, 0.0)));
    }
    if(keyDown[SDLK_UP]) {
        EMIT_EVENT(new Event::PlayerMovement(Math::Point(0.0, +0.01)));
    }
    if(keyDown[SDLK_DOWN]) {
        EMIT_EVENT(new Event::PlayerMovement(Math::Point(0.0, -0.01)));
    }
}

}  // namespace SDL
}  // namespace Project
