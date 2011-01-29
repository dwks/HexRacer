#ifndef PROJECT_SDL__INPUT_MANAGER_H
#define PROJECT_SDL__INPUT_MANAGER_H

#include "SDL_events.h"
#include "SDL_keysym.h"

#include "timing/TimedSubsystem.h"

#include "PlayerManager.h"
#include "ClientData.h"
#include "JoystickManager.h"

namespace Project {
namespace SDL {

class InputManager : public Timing::TimedSubsystem {
private:
    bool keyDown[SDLK_LAST];
    ClientData *clientData;
    JoystickManager *joystick;
public:
    InputManager(int ms, ClientData *clientData);
    ~InputManager();
    
    void init();
    
    void handleEvent(SDL_Event *event);
    
    virtual void doAction(unsigned long currentTime);
    
    void doPausedChecks();
private:
    void handlePaint();
    void handleJoystick();
};

}  // namespace SDL
}  // namespace Project

#endif
