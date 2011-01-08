#include <cctype>
#include "SDL.h"
#include "GL/gl.h"

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

void SDLMain::run() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_HWSURFACE | SDL_OPENGL);
    
    for(;;) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                if(std::isprint(event.key.keysym.sym)) {
                    LOG2(SDL, INPUT, "Key pressed: '" << char(event.key.keysym.sym) << "'");
                }
                else {
                    LOG2(SDL, INPUT, "Extended key pressed");
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                LOG2(SDL, INPUT, "Mouse button " << int(event.button.button) << " clicked");
                break;
            }
        }
        
        render();
        
        SDL_GL_SwapBuffers();
        
        SDL_Delay(10);
    }
}

void SDLMain::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslated(0.0, 0.0, -1.0);
    
    /*glTranslated(
        -translation.getX(),
        -translation.getY(),
        -translation.getZ());*/
    
    //glRotated(angle, 0.0, 1.0, 0.0);
    //glScaled(scale, scale, scale);
    
    // add a light
    {
        static GLfloat pos[] = {0.0, 0.0, 2.0, 1.0};
        
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
    }
    
    glBegin(GL_TRIANGLES);
    OpenGL::MathWrapper::glVertex(Math::Point(0.0, 0.1, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(-0.1, 0.0, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(+0.1, 0.0, 0.0));
    glEnd();
    
    glFlush();
}

}  // namespace SDL
}  // namespace Project
