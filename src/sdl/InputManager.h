#ifndef PROJECT_SDL__INPUT_MANAGER_H
#define PROJECT_SDL__INPUT_MANAGER_H

#include "timing/TimedSubsystem.h"

#include "ClientData.h"
//#include "JoystickManager.h"
#include "world/PlayerManager.h"

#include "input/InputMapper.h"

namespace Project {
namespace SDL {

class InputManager : public Timing::TimedSubsystem {

private:

    ClientData *clientData;
    World::PlayerManager *playerManager;
    //JoystickManager *joystick;
	Input::InputMapper *inputMapper;

	double debugCamera;

public:

    InputManager(int ms, ClientData *clientData,
        World::PlayerManager *playerManager);
    ~InputManager();
    
    void init();
    
    virtual void doAction(unsigned long currentTime);
    
    void doPausedChecks();


private:
    void handlePaint();
};

}  // namespace SDL
}  // namespace Project

#endif
