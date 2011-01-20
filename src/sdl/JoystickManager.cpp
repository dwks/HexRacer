#include <climits>
#include "SDL.h"
#include <math.h>
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

void JoystickManager::open() {
    if(SDL_NumJoysticks() > 0) {
        LOG2(SDL, INPUT, "Found " << SDL_NumJoysticks() << " joysticks, "
            "using first one");
        //SDL_JoystickEventState(SDL_ENABLE);  // for generating events
        joystick = SDL_JoystickOpen(0);
    }
    else {
        LOG2(SDL, INPUT, "No joystick found");
    }
}

double JoystickManager::getNormalizedAxisValue(int axis, double deadzone) {
    if(joystick == NULL) return 0.0;
    
    int value = SDL_JoystickGetAxis(joystick, axis);
    
    double normalized = (double(value) - SHRT_MIN) / (SHRT_MAX - SHRT_MIN);
    normalized = (normalized - 0.5) * 2;

	if (fabs(normalized) < deadzone)
		return 0.0f;
	else
		return normalized;
}

}  // namespace SDL
}  // namespace Project
