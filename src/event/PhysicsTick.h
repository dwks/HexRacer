#ifndef PROJECT_EVENT__PHYSICS_TICK_H
#define PROJECT_EVENT__PHYSICS_TICK_H

#include "EventBase.h"

namespace Project {
namespace Event {

class PhysicsTick : public EventBase {
private:
    double elapsed;
public:
    PhysicsTick() : elapsed(0.0) {}
    PhysicsTick(double elapsed) : elapsed(elapsed) {}
    
    double getElapsed() const { return elapsed; }
    
    virtual EventType::type_t getType() const
        { return EventType::PHYSICS_TICK; }
};

}  // namespace Event
}  // namespace Project

#endif
