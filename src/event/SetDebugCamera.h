#ifndef PROJECT_EVENT__SET_DEBUG_CAMERA_H
#define PROJECT_EVENT__SET_DEBUG_CAMERA_H

#include "EventBase.h"

namespace Project {
namespace Event {

class SetDebugCamera : public EventBase {
private:
    bool on;
public:
    SetDebugCamera() : on(false) {}
    SetDebugCamera(bool on) : on(on) {}
    
    bool getOn() const { return on; }
    
    virtual EventType::type_t getType() const
        { return EventType::SET_DEBUG_CAMERA; }
};

}  // namespace Event
}  // namespace Project

#endif
