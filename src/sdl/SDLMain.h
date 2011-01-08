#ifndef PROJECT_SDL__SDLMAIN_H
#define PROJECT_SDL__SDLMAIN_H

namespace Project {
namespace SDL {

class SDLMain {
public:
    SDLMain();
    ~SDLMain();
    
    void run();
private:
    void render();
};

}  // namespace SDL
}  // namespace Project

#endif
