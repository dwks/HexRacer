#ifndef PROJECT_SDL__SDLMAIN_H
#define PROJECT_SDL__SDLMAIN_H

#include "opengl/Trackball.h"
#include "Projector.h"

#define FIELD_OF_VIEW 60
#define SDL_INIT_FLAGS (SDL_HWSURFACE | SDL_OPENGL | SDL_RESIZABLE)

namespace Project {
namespace SDL {

class SDLMain {
private:
    OpenGL::Trackball *trackball;
    Projector projector;
public:
    SDLMain();
    ~SDLMain();
    
    void run();
private:
    void resizeGL(int width, int height);
    void render();
};

}  // namespace SDL
}  // namespace Project

#endif
