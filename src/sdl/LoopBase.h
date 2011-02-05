#ifndef PROJECT_SDL__LOOP_BASE_H
#define PROJECT_SDL__LOOP_BASE_H

#include "SDL_events.h"

#include "Point2D.h"

namespace Project {
namespace SDL {

class LoopBase {
public:
    virtual ~LoopBase() {}
    
    virtual void construct() = 0;
    
    virtual void handleEvent(SDL_Event *event) = 0;
    virtual void render() = 0;
    
    virtual void setProjection(const Point2D &size) = 0;
};

}  // namespace SDL
}  // namespace Project

#endif
