#ifndef PROJECT_SDL__INPUT_MANAGER_H
#define PROJECT_SDL__INPUT_MANAGER_H

#include "SDL_events.h"
#include "SDL_keysym.h"

#include "PlayerManager.h"

namespace Project {
namespace SDL {

class InputManager {
private:
    bool keyDown[SDLK_LAST];
    PlayerManager *playerManager;
public:
    InputManager(PlayerManager *playerManager);
    
    void handleEvent(SDL_Event *event);
    
    void advanceToNextFrame();
};

}  // namespace SDL
}  // namespace Project

#endif
