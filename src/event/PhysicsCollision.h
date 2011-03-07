#ifndef PROJECT_EVENT__PHYSICS_COLLISION_H
#define PROJECT_EVENT__PHYSICS_COLLISION_H

#include "EventBase.h"
#include "math/Point.h"

namespace Project {
namespace Event {

class PhysicsCollision : public EventBase {
private: 
    Math::Point location;
    float impulse;
public:
    PhysicsCollision() : location(Math::Point()),impulse(0.0) {}
    PhysicsCollision(Math::Point location, float impulse) : location(location),impulse(impulse) {}
    
    Math::Point getLocation() const { return location; }
    float getImpulse() const { return impulse; }
    
    virtual EventType::type_t getType() const
        { return EventType::PHYSICS_COLLISION; }
};

}  // namespace Event
}  // namespace Project

#endif
