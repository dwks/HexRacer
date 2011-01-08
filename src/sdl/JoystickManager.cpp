#include <climits>
#include "SDL.h"

#include "JoystickManager.h"

namespace Project {
namespace SDL {

JoystickManager::JoystickManager() {
    joystick = NULL;
}

JoystickManager::~JoystickManager() {
    if(joystick) {
        SDL_JoystickClose(joystick);
    }
}

void JoystickManager::open() {
    if(SDL_NumJoysticks() > 0) {
        //SDL_JoystickEventState(SDL_ENABLE);  // for generating events
        joystick = SDL_JoystickOpen(0);
    }
}

double JoystickManager::getNormalizedAxisValue(int axis) {
    if(joystick == NULL) return 0.0;
    
    int value = SDL_JoystickGetAxis(joystick, axis);
    
    double normalized = (double(value) - SHRT_MIN) / (SHRT_MAX - SHRT_MIN);
    normalized = (normalized - 0.5) * 2;
    
    return normalized;
}

}  // namespace SDL
}  // namespace Project
