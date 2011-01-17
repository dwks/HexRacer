#ifndef PROJECT_SDL__JOYSTICK_MANAGER_H
#define PROJECT_SDL__JOYSTICK_MANAGER_H

#include "SDL_joystick.h"

namespace Project {
namespace SDL {

/** Opens and reads from a joystick, if the user has one.
*/
class JoystickManager {
private:
    SDL_Joystick *joystick;
public:
    JoystickManager();
    ~JoystickManager();
    
    void open();
    
    /** Returns the value of the joystick axis @a axis, normalized to the range
        [-1.0, 1.0]. This value will be 0.0 if the joystick was not detected.
    */
    double getNormalizedAxisValue(int axis, double deadzone = 0.25f);
};

}  // namespace SDL
}  // namespace Project

#endif
