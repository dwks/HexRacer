#ifndef PROJECT_SDL__GAME_LOOP_H
#define PROJECT_SDL__GAME_LOOP_H

#include <string>

#include "LoopBase.h"

namespace Project {
namespace SDL {

class GameLoop : public LoopBase {
public:
    GameLoop(const std::string &host, unsigned short port);
    
    virtual void construct();
    
    virtual void handleEvent(SDL_Event *event);
    virtual void render();
    
    virtual void setProjection(const Point2D &size);
};

}  // namespace SDL
}  // namespace Project

#endif
