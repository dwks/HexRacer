#ifndef PROJECT_EVENT__BASIC_WORLD_CONSTRUCTED_H
#define PROJECT_EVENT__BASIC_WORLD_CONSTRUCTED_H

#include "EventBase.h"
#include "world/BasicWorld.h"

namespace Project {
namespace Event {

class BasicWorldConstructed : public EventBase {
private:
    World::BasicWorld *world;
public:
    BasicWorldConstructed(World::BasicWorld *world = NULL) : world(world) {}
    
    World::BasicWorld *getBasicWorld() const { return world; }
    
    virtual EventType::type_t getType() const
        { return EventType::BASIC_WORLD_CONSTRUCTED; }
};

}  // namespace Event
}  // namespace Project

#endif
