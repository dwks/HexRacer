#ifndef PROJECT_SDL__SDLMAIN_LOOP_H
#define PROJECT_SDL__SDLMAIN_LOOP_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include "LoopBase.h"
#include "Projector.h"

#include "event/QuitEvent.h"
#include "event/JoinGame.h"

#include "timing/AccelControl.h"

namespace Project {
namespace SDL {

class MenuLoop;

class SDLMainLoop {
private:
    int sdl_init_flags;
    bool quit;
    MenuLoop *menuLoop;
    LoopBase *loop;
    
    Projector projector;
    
    boost::shared_ptr<Timing::AccelControl> accelControl;
private:
    class QuitObserver : public Event::TypedObserver<Event::QuitEvent> {
    private:
        SDLMainLoop *mainLoop;
    public:
        QuitObserver(SDLMainLoop *mainLoop) : mainLoop(mainLoop) {}
        
        virtual void observe(Event::QuitEvent *event);
    };
    
    class JoinGameObserver : public Event::TypedObserver<Event::JoinGame> {
    private:
        SDLMainLoop *mainLoop;
    public:
        JoinGameObserver(SDLMainLoop *mainLoop) : mainLoop(mainLoop) {}
        
        virtual void observe(Event::JoinGame *event);
    };
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
private:
    void handleEvents();
    void doRender();
};

}  // namespace SDL
}  // namespace Project

#endif
