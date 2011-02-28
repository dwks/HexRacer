#include <climits>
#include <cmath>
#include "SDL.h"
using namespace std;

#include "JoystickManager.h"
#include "log/Logger.h"

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

bool JoystickManager::open() {

    if (SDL_NumJoysticks() > 0) {
        LOG2(SDL, INPUT, "Found " << SDL_NumJoysticks() << " joysticks, "
            "using first one");
        //SDL_JoystickEventState(SDL_ENABLE);  // for generating events
        joystick = SDL_JoystickOpen(0);
		return true;
    }
    else {
        LOG2(SDL, INPUT, "No joystick found");
		return false;
    }

}

double JoystickManager::getNormalizedAxisValue(int axis, double deadzone) {
    if (joystick == NULL) return 0.0;
    
    int value = SDL_JoystickGetAxis(joystick, axis);
    
    double normalized = (double(value) - SHRT_MIN) / (SHRT_MAX - SHRT_MIN);
    normalized = (normalized - 0.5) * 2;
    
    //LOG2(SDL, INPUT, axis << " is at " << normalized);
    
    return (std::fabs(normalized) < deadzone) ? 0.0 : normalized;
}

bool JoystickManager::getButtonDown(int button) {
	return (joystick != NULL && SDL_JoystickGetButton(joystick, button) != 0);
}

}  // namespace SDL
}  // namespace Project
