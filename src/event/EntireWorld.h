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
        ar & laps;
    }
private:
    Object::World *world;
    Object::PlayerList *playerList;
    int laps;
public:
    EntireWorld() : world(0), playerList(0), laps(0) {}
    EntireWorld(Object::World *world, Object::PlayerList *playerList, int laps)
        : world(world), playerList(playerList), laps(laps) {}
    
    Object::World *getWorld() { return world; }
    Object::PlayerList *getPlayerList() { return playerList; }
    int getLaps() const { return laps; }
    
    virtual EventType::type_t getType() const
        { return EventType::ENTIRE_WORLD; }
};

}  // namespace Event
}  // namespace Project

#endif
