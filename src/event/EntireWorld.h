#ifndef PROJECT_EVENT__ENTIRE_WORLD_H
#define PROJECT_EVENT__ENTIRE_WORLD_H

#include "EventBase.h"
#include "object/World.h"
#include "object/PlayerList.h"

namespace Project {
namespace Event {

class EntireWorld : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & world;
        ar & playerList;
    }
private:
    Object::World *world;
    Object::PlayerList *playerList;
public:
    EntireWorld() : world(0) {}
    EntireWorld(Object::World *world, Object::PlayerList *playerList)
        : world(world), playerList(playerList) {}
    
    Object::World *getWorld() { return world; }
    Object::PlayerList *getPlayerList() { return playerList; }
    
    virtual EventType::type_t getType() const
        { return EventType::ENTIRE_WORLD; }
};

}  // namespace Event
}  // namespace Project

#endif
