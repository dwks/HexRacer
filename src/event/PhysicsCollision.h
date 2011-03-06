#ifndef PROJECT_EVENT__PHYSICS_COLLISION_H
#define PROJECT_EVENT__PHYSICS_COLLISION_H

#include "EventBase.h"
#include "math/Point.h"

namespace Project {
namespace Event {

class PhysicsCollision : public EventBase {
private: 
    Math::Point location;
public:
    PhysicsCollision() : location(Math::Point()) {}
    PhysicsCollision(Math::Point location) : location(location) {}
    
    Math::Point getLocation() const { return location; }
    
    virtual EventType::type_t getType() const
        { return EventType::PHYSICS_COLLISION; }
};

}  // namespace Event
}  // namespace Project

#endif
