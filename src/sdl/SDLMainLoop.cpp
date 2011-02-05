#include "SDLMainLoop.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "opengl/OpenGL.h"

#include "MenuLoop.h"

#include "event/ObserverList.h"

#include "settings/SettingsManager.h"

#include "log/Logger.h"

namespace Project {
namespace SDL {

SDLMainLoop::SDLMainLoop() {
    quit = false;
    ADD_OBSERVER(new QuitObserver(this));
    
    initSDL();
    
    loop = new MenuLoop();
}

SDLMainLoop::~SDLMainLoop() {
    TTF_Quit();
    SDL_Quit();
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
    
    SDL_WM_SetCaption("The Project", NULL);
    
    int width = GET_SETTING("display.width", 0);
    int height = GET_SETTING("display.height", 0);
    int bpp = GET_SETTING("display.bpp", 0);
    
    sdl_init_flags = SDL_HWSURFACE | SDL_OPENGL | SDL_RESIZABLE;
    if(GET_SETTING("display.fullscreen", 0)) {
        sdl_init_flags |= SDL_FULLSCREEN;
    }
    
    SDL_SetVideoMode(width, height, bpp, sdl_init_flags);
}

void SDLMainLoop::resizeGL(int width, int height) {
    // in case of divide by zero
    if(height == 0) height = 1;
    
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
    
    while(!quit) {
        handleEvents();
        
        loop->render();
        glFlush();
        SDL_GL_SwapBuffers();
        
        {
            static const int RATE = 10;
            Uint32 thisTime = SDL_GetTicks();
            int timeTakenSoFar = static_cast<int>(thisTime - lastTime);
            if(timeTakenSoFar < RATE) {
                SDL_Delay(RATE - timeTakenSoFar);
            }
            while(lastTime < thisTime) lastTime += RATE;
        }
    }
}

void SDLMainLoop::handleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            setQuit();
            break;
        case SDL_VIDEORESIZE:
            SDL_SetVideoMode(event.resize.w, event.resize.h,
                0, sdl_init_flags);
            resizeGL(event.resize.w, event.resize.h);
            projector.setCurrentDimensions(
                Point2D(event.resize.w, event.resize.h));
            break;
        }
        
        loop->handleEvent(&event);
    }
}

}  // namespace SDL
}  // namespace Project
