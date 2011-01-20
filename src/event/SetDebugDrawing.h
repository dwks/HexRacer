#ifndef PROJECT_EVENT__SET_DEBUG_DRAWING_H
#define PROJECT_EVENT__SET_DEBUG_DRAWING_H

#include "EventBase.h"

namespace Project {
namespace Event {

class SetDebugDrawing : public EventBase {
private:
    bool on;
public:
    SetDebugDrawing() : on(false) {}
    SetDebugDrawing(bool on) : on(on) {}
    
    bool getOn() const { return on; }
    
    virtual EventType::type_t getType() const
        { return EventType::SET_DEBUG_DRAWING; }
};

}  // namespace Event
}  // namespace Project

#endif
