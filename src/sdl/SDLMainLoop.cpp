#include "SDLMainLoop.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "opengl/OpenGL.h"

#include "MenuLoop.h"
#include "GameLoop.h"

#include "event/EventSystem.h"
#include "event/DoDisconnect.h"

#include "settings/SettingsManager.h"

#include "log/Logger.h"

namespace Project {
namespace SDL {

void SDLMainLoop::quitHandler(Event::QuitEvent *event) {
    if(!currentlyUsingMenuLoop()) {
        EMIT_EVENT(new Event::DoDisconnect());
        useMenuLoop();
    }
    else {
        setQuit();
    }
}

void SDLMainLoop::changeScreenModeHandler(Event::ChangeScreenMode *event) {
    int width = event->getWidth();
    int height = event->getHeight();
    int bpp = event->getBPP();
    bool fullscreen = event->getFullscreen();
    
#ifndef WIN32
    // special case: if just toggling fullscreen, and the operating system
    // supports it, can just call ToggleFullScreen instead of setting the mode
    if(width == GET_SETTING("display.width", 0)
        && height == GET_SETTING("display.height", 0)
        && bpp == GET_SETTING("display.bpp", 0)
        && fullscreen != GET_SETTING("display.fullscreen", 0)) {
        
        if(SDL_WM_ToggleFullScreen(SDL_GetVideoSurface())) return;
    }
#endif
    
    Settings::SettingsManager::getInstance()->set(
        "display.width", Misc::StreamAsString() << width);
    Settings::SettingsManager::getInstance()->set(
        "display.height", Misc::StreamAsString() << height);
    Settings::SettingsManager::getInstance()->set(
        "display.bpp", Misc::StreamAsString() << bpp);
    Settings::SettingsManager::getInstance()->set(
        "display.fullscreen", Misc::StreamAsString() << fullscreen);
    
    if(fullscreen) {
        sdl_init_flags |= SDL_FULLSCREEN;
    }
    else {
        sdl_init_flags &= ~SDL_FULLSCREEN;
    }
    
    SDL_SetVideoMode(width, height, bpp, this->sdl_init_flags);
    resizeGL(width, height);
}

void SDLMainLoop::joinGameHandler(Event::JoinGame *event) {
    gameLoop = new GameLoop();
    if(!gameLoop->tryConnect(event->getHost(), event->getPort())) {
        delete gameLoop;
        gameLoop = NULL;
        
        Widget::TextWidget *error = dynamic_cast<Widget::TextWidget *>(
            menuLoop->getGUI()->getWidget("connect/error"));
        if(error) {
            error->setText(Misc::StreamAsString()
                << "Could not connect to " << event->getHost()
                << ":" << event->getPort());
        }
        
        /*if(menuLoop->getGUI()->getCurrentScreen()->getName() == "loading") {
            menuLoop->getGUI()->popScreen();
        }*/
        
        return;
    }
    else {
        event->setSuccessful();
        
        menuLoop->setGameWorld(gameLoop->getGameWorld());
    }
}

void SDLMainLoop::startingGameHandler(Event::StartingGame *event) {
    if(event->getStatus() == Event::StartingGame::LOADING_MAP) {
        menuLoop->soundSystem->cleanUp();
        gameLoop->resumeConnect();
        gameLoop->construct();
    }
}

void SDLMainLoop::gameStageChangedHandler(Event::GameStageChanged *event) {
    if(event->getStage() == World::WorldSetup::START_COUNTDOWN) {
        gameLoop->setGuiPointers(
            menuLoop->getGUI(),
            menuLoop->getGUIInput());
        menuLoop->getGUI()->popScreen();
        //menuLoop->getGUI()->pushScreen("running");
        EMIT_EVENT(new Event::SwitchToScreen("running"));
        
        // set up camera if necessary
        gameLoop->setProjection(Point2D(
            SDL_GetVideoSurface()->w,
            SDL_GetVideoSurface()->h));
        
        //Timing::AccelControl::getInstance()->setPauseSkipDirectly(SDL_GetTicks());
        useLoopBase(gameLoop);
    }
}

SDLMainLoop::SDLMainLoop() {
    quit = false;
    
    // it's dangerous to add observers inside a constructor, but in this case
    // we know that SDLMainLoop was not constructed from within an event, so
    // it's okay.
    METHOD_OBSERVER(&SDLMainLoop::quitHandler);
    METHOD_OBSERVER(&SDLMainLoop::changeScreenModeHandler);
    METHOD_OBSERVER(&SDLMainLoop::joinGameHandler);
    METHOD_OBSERVER(&SDLMainLoop::startingGameHandler);
    METHOD_OBSERVER(&SDLMainLoop::gameStageChangedHandler);
    
    initSDL();
    initOpenGL();
    
    menuLoop = new MenuLoop();
    gameLoop = NULL;
    loop = menuLoop;
}

SDLMainLoop::~SDLMainLoop() {
    delete menuLoop;
    if(!currentlyUsingMenuLoop()) delete loop;
    
    TTF_Quit();
    SDL_Quit();
}

void SDLMainLoop::useLoopBase(LoopBase *loop) {
    this->loop = loop;
}

void SDLMainLoop::useMenuLoop() {
    delete this->loop;  // delete the other menu, whatever it is
    gameLoop = NULL;
    
    menuLoop->getGUI()->popScreen("main");
    this->loop = menuLoop;
    menuLoop->instantiateSound();
}

bool SDLMainLoop::currentlyUsingMenuLoop() {
    return loop == menuLoop;
}

void SDLMainLoop::initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        LOG2(SDL, ERROR, "Can't init SDL: " << SDL_GetError());
    }
    
    if(TTF_Init() < 0) {
        LOG2(SDL, ERROR, "Can't init SDL_ttf: " << TTF_GetError());
    }
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_WM_SetCaption("HexRacer", NULL);
    
    int width = GET_SETTING("display.width", 0);
    int height = GET_SETTING("display.height", 0);
    int bpp = GET_SETTING("display.bpp", 0);
    
    sdl_init_flags = SDL_HWSURFACE | SDL_OPENGL | SDL_RESIZABLE;
    if(GET_SETTING("display.fullscreen", 0)) {
        sdl_init_flags |= SDL_FULLSCREEN;
    }
    
    SDL_SetVideoMode(width, height, bpp, sdl_init_flags);
    projector.setCurrentDimensions(Point2D(width, height));
}

void SDLMainLoop::initOpenGL() {
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_RESCALE_NORMAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // note: must match TextWidget
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
}

void SDLMainLoop::resizeGL(int width, int height) {
    LOG(SDL, "Resizing viewport to " << width << " by " << height);
    
    // in case of divide by zero
    if (height == 0) height = 1;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // set repaint rect to encompass entire window
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    projector.setCurrentDimensions(Point2D(width, height));
    if(loop) loop->setProjection(Point2D(width, height));
    
    glMatrixMode(GL_MODELVIEW);
}

void SDLMainLoop::run() {
    loop->construct();
    
    LOG2(GLOBAL, PROGRESS, "Entering main loop");
    Uint32 lastTime = SDL_GetTicks();
    
    accelControl = boost::shared_ptr<Timing::AccelControl>(
        new Timing::AccelControl());
    accelControl->setPauseSkipDirectly(lastTime);

	inputManager = boost::shared_ptr<Input::GlobalInputManager>(
        new Input::GlobalInputManager());
    
    while(!quit) {
        handleEvents();
        
        loop->miscellaneous();
        
        doRender();
        
        {
            static const int RATE = 10;
            Uint32 thisTime = SDL_GetTicks();
            int timeTakenSoFar = static_cast<int>(thisTime - lastTime);
            if(timeTakenSoFar < RATE) {
                SDL_Delay(RATE - timeTakenSoFar);
            }
            while(lastTime < thisTime) lastTime += RATE;
        }
        
        accelControl->clearPauseSkip();
    }
    
    LOG2(GLOBAL, PROGRESS, "Exiting main loop");
}

void SDLMainLoop::handleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            EMIT_EVENT(new Event::QuitEvent());
            break;
        case SDL_VIDEORESIZE:
            SDL_SetVideoMode(event.resize.w, event.resize.h,
                0, sdl_init_flags);
            resizeGL(event.resize.w, event.resize.h);
            /*projector.setCurrentDimensions(
                Point2D(event.resize.w, event.resize.h));*/
            break;
        }
        
        loop->handleEvent(&event);
		inputManager->getInputMapper()->handleEvent(&event);
    }
}

void SDLMainLoop::doRender() {
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    loop->render();
    
    glFlush();
    SDL_GL_SwapBuffers();
    
    loop->postRender();
}

}  // namespace SDL
}  // namespace Project
