#ifndef PROJECT_EVENT__MODIFY_CAMERA_H
#define PROJECT_EVENT__MODIFY_CAMERA_H

#include "EventBase.h"

namespace Project {
namespace Event {

class ModifyCamera : public EventBase {
public:
    enum Modification {
        FOCUS_PLAYER
    };
private:
    Modification mod;
    double value;
public:
    ModifyCamera() : mod(FOCUS_PLAYER), value(0.0) {}
    ModifyCamera(Modification mod, double value) : mod(mod), value(value) {}
    
    Modification getModification() const { return mod; }
    double getValue() const { return value; }
    
    virtual EventType::type_t getType() const
        { return EventType::MODIFY_CAMERA; }
};

}  // namespace Event
}  // namespace Project

#endif
