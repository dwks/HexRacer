#ifndef PROJECT_SDL__INPUT_MANAGER_H
#define PROJECT_SDL__INPUT_MANAGER_H

#include "SDL_events.h"
#include "SDL_keysym.h"

#include "timing/TimedSubsystem.h"

#include "ClientData.h"
#include "JoystickManager.h"
#include "world/PlayerManager.h"

#include "input/InputMapper.h"

namespace Project {
namespace SDL {

class InputManager : public Timing::TimedSubsystem {

public:
	enum PresetMapping { NO_JOYSTICK, XB360_WINDOWS };

private:

    ClientData *clientData;
    World::PlayerManager *playerManager;
    JoystickManager *joystick;
	Input::InputMapper *inputMapper;

	double debugCamera;

public:

    InputManager(int ms, ClientData *clientData,
        World::PlayerManager *playerManager);
    ~InputManager();
    
    void init();
    
    void handleEvent(SDL_Event *event);
    
    virtual void doAction(unsigned long currentTime);
    
    void doPausedChecks();

	void setInputMappings(PresetMapping mapping = NO_JOYSTICK);

private:
    void handlePaint();
    //void handleJoystick();
};

}  // namespace SDL
}  // namespace Project

#endif
