#ifndef PROJECT_EVENT__CHANGE_SCREEN_MODE_H
#define PROJECT_EVENT__CHANGE_SCREEN_MODE_H

#include "EventBase.h"

namespace Project {
namespace Event {

class ChangeScreenMode : public EventBase {
private:
    int width, height, bpp;
public:
    ChangeScreenMode() : width(0), height(0), bpp(0) {}
    ChangeScreenMode(int width, int height, int bpp)
        : width(width), height(height), bpp(bpp) {}
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getBPP() const { return bpp; }
    
    virtual EventType::type_t getType() const
        { return EventType::CHANGE_SCREEN_MODE; }
};

}  // namespace Event
}  // namespace Project

#endif
