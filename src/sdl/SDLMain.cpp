#include <cctype>
#include <cmath>

#include "SDL.h"
#include "opengl/OpenGL.h"
#include "GL/glu.h"

#include "SDLMain.h"
#include "log/Logger.h"

#include "math/Point.h"
#include "opengl/MathWrapper.h"

#include "event/ObserverList.h"
#include "event/PlayerMovement.h"

#include "config.h"

namespace Project {
namespace SDL {

SDLMain::SDLMain() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        LOG2(SDL, ERROR, "Can't init SDL: " << SDL_GetError());
    }
}

SDLMain::~SDLMain() {
    SDL_Quit();
}

void SDLMain::resizeGL(int width, int height) {
    // in case of divide by zero
    if(height == 0) height = 1;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // set repaint rect to encompass entire window
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // use perspective projection
    double aspect_ratio = static_cast<double>(width) / height;
    gluPerspective(FIELD_OF_VIEW, aspect_ratio, 0.01, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

void SDLMain::run() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_INIT_FLAGS);
    
    projector.setCurrentDimensions(Point2D(WIDTH, HEIGHT));
    resizeGL(WIDTH, HEIGHT);
    
    trackball = new OpenGL::Trackball();
    
    joystick = new JoystickManager();
    joystick->open();
    
    network = new NetworkPortal();
    network->connectTo("localhost", 1820);
    
    playerManager = new PlayerManager(0);
    
    bool quit = false;
    while(!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_VIDEORESIZE:
                SDL_SetVideoMode(event.resize.w, event.resize.h,
                    0, SDL_INIT_FLAGS);
                resizeGL(event.resize.w, event.resize.h);
                projector.setCurrentDimensions(
                    Point2D(event.resize.w, event.resize.h));
                break;
            case SDL_KEYDOWN:
                /*if(event.key.keysym.sym < 128) {
                    LOG2(SDL, INPUT, "Key pressed: '" << char(event.key.keysym.sym) << "'");
                }
                else {
                    //LOG2(SDL, INPUT, "Extended key pressed");
                }*/
                
                handleKeyDown(&event);
                
                break;
            case SDL_MOUSEBUTTONDOWN:
                /*LOG2(SDL, INPUT, "Mouse button " << int(event.button.button) << " pressed "
                    << "at " << event.button.x << "," << event.button.y);*/
                trackball->setMouseStartAt(projector.screenToGL(
                    Point2D(event.button.x, event.button.y)));
                break;
            case SDL_MOUSEMOTION:
                if(event.motion.state & SDL_BUTTON(1)) {
                    //LOG2(SDL, INPUT, "Mouse moved to " << event.motion.x << "," << event.motion.y);
                    trackball->setMouseCurrentAt(projector.screenToGL(
                        Point2D(event.motion.x, event.motion.y)));
                }
                break;
            case SDL_MOUSEBUTTONUP:
                //LOG2(SDL, INPUT, "Mouse button " << int(event.button.button) << " released");
                break;
            }
        }
        
        network->checkNetwork();
        
        handleJoystick();
        
        render();
        playerManager->render();
        
        SDL_GL_SwapBuffers();
        
        SDL_Delay(10);
    }
    
    delete trackball;
    delete joystick;
    delete network;
}

void SDLMain::handleJoystick() {
    double x = joystick->getNormalizedAxisValue(0);
    double y = joystick->getNormalizedAxisValue(1);
    
    if(std::fabs(x) > 1e-3 || std::fabs(y) > 1e-3) {
        //LOG(SDL, "Move joystick by " << x << "," << y);
        trackball->setMouseStartAt(Math::Point(0.0, 0.0));
        
        Math::Point translation;
        translation.setX(x * 0.1);
        translation.setY(-y * 0.1);
        
        trackball->setMouseCurrentAt(translation);
    }
}

void SDLMain::handleKeyDown(SDL_Event *event) {
    switch(event->key.keysym.sym) {
    case SDLK_LEFT:
        EMIT_EVENT(new Event::PlayerMovement(Math::Point(-0.1, 0.0)));
        break;
    case SDLK_RIGHT:
        EMIT_EVENT(new Event::PlayerMovement(Math::Point(+0.1, 0.0)));
        break;
    case SDLK_UP:
        EMIT_EVENT(new Event::PlayerMovement(Math::Point(0.0, +0.1)));
        break;
    case SDLK_DOWN:
        EMIT_EVENT(new Event::PlayerMovement(Math::Point(0.0, -0.1)));
        break;
    default:
        break;
    }
}

void SDLMain::render() {
    glMatrixMode(GL_MODELVIEW);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslated(0.0, 0.0, -1.0);
    
    trackball->applyTransform();
    
    glBegin(GL_TRIANGLES);
    OpenGL::MathWrapper::glVertex(Math::Point(0.0, 0.1, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(-0.1, 0.0, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(+0.1, 0.0, 0.0));
    glEnd();
    
    glFlush();
}

}  // namespace SDL
}  // namespace Project
