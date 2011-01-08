#include <cctype>
#include "SDL.h"
#include "opengl/OpenGL.h"
#include "GL/glu.h"

#include "SDLMain.h"
#include "log/Logger.h"

#include "math/Point.h"
#include "opengl/MathWrapper.h"

#include "config.h"

namespace Project {
namespace SDL {

SDLMain::SDLMain() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
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
    
    for(;;) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                return;
            case SDL_VIDEORESIZE:
                SDL_SetVideoMode(event.resize.w, event.resize.h,
                    0, SDL_INIT_FLAGS);
                resizeGL(event.resize.w, event.resize.h);
                projector.setCurrentDimensions(
                    Point2D(event.resize.w, event.resize.h));
                break;
            case SDL_KEYDOWN:
                if(std::isprint(event.key.keysym.sym)) {
                    LOG2(SDL, INPUT, "Key pressed: '" << char(event.key.keysym.sym) << "'");
                }
                else {
                    LOG2(SDL, INPUT, "Extended key pressed");
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                LOG2(SDL, INPUT, "Mouse button " << int(event.button.button) << " pressed "
                    << "at " << event.button.x << "," << event.button.y);
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
                LOG2(SDL, INPUT, "Mouse button " << int(event.button.button) << " released");
                break;
            }
        }
        
        render();
        
        SDL_GL_SwapBuffers();
        
        SDL_Delay(10);
    }
    
    delete trackball;
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
