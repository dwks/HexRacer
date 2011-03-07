#ifndef PROJECT_EVENT__CHANGE_SCREEN_MODE_H
#define PROJECT_EVENT__CHANGE_SCREEN_MODE_H

#include "EventBase.h"

namespace Project {
namespace Event {

class ChangeScreenMode : public EventBase {
private:
    int width, height, bpp;
    bool fullscreen;
public:
    ChangeScreenMode() : width(0), height(0), bpp(0), fullscreen(false) {}
    ChangeScreenMode(int width, int height, int bpp, bool fullscreen)
        : width(width), height(height), bpp(bpp), fullscreen(fullscreen) {}
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getBPP() const { return bpp; }
    bool getFullscreen() const { return fullscreen; }
    
    virtual EventType::type_t getType() const
        { return EventType::CHANGE_SCREEN_MODE; }
};

}  // namespace Event
}  // namespace Project

#endif
