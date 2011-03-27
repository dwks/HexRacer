#ifndef PROJECT_SDL__SDLMAIN_LOOP_H
#define PROJECT_SDL__SDLMAIN_LOOP_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include "LoopBase.h"
#include "Projector.h"

#include "event/QuitEvent.h"
#include "event/ChangeScreenMode.h"
#include "event/JoinGame.h"
#include "event/StartingGame.h"
#include "event/TypedObserver.h"
#include "event/Enabler.h"
#include "event/GameStageChanged.h"

#include "timing/AccelControl.h"
#include "input/GlobalInputManager.h"

namespace Project {
namespace SDL {

class MenuLoop;
class GameLoop;

class SDLMainLoop : public Event::Enabler {
private:
    int sdl_init_flags;
    bool quit;
    MenuLoop *menuLoop;
    GameLoop *gameLoop;
    LoopBase *loop;
    
    Projector projector;
    
    boost::shared_ptr<Timing::AccelControl> accelControl;
	boost::shared_ptr<Input::GlobalInputManager> inputManager;
protected:
    void quitHandler(Event::QuitEvent *event);
    void changeScreenModeHandler(Event::ChangeScreenMode *event);
    void joinGameHandler(Event::JoinGame *event);
    void startingGameHandler(Event::StartingGame *event);
    void gameStageChangedHandler(Event::GameStageChanged *event);
public:
    SDLMainLoop();
    ~SDLMainLoop();
    
    virtual void resizeGL(int width, int height);
    
    void run();
    
    void useLoopBase(LoopBase *loop);
    void useMenuLoop();
    bool currentlyUsingMenuLoop();
    
    void setQuit() { quit = true; }
private:
    void initSDL();
    void initOpenGL();
private:
    void handleEvents();
    void doRender();
};

}  // namespace SDL
}  // namespace Project

#endif
