#ifndef PROJECT_EVENT__ENTIRE_WORLD_H
#define PROJECT_EVENT__ENTIRE_WORLD_H

#include "EventBase.h"
#include "object/World.h"

namespace Project {
namespace Event {

class EntireWorld : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & world;
    }
private:
    Object::World *world;
public:
    EntireWorld() : world(0) {}
    EntireWorld(Object::World *world) : world(world) {}
    
    Object::World *getWorld() { return world; }
    
    virtual EventType::type_t getType() const
        { return EventType::ENTIRE_WORLD; }
};

}  // namespace Event
}  // namespace Project

#endif
