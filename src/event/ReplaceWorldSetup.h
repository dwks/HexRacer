#ifndef PROJECT_EVENT__REPLACE_WORLD_SETUP_H
#define PROJECT_EVENT__REPLACE_WORLD_SETUP_H

#include "EventBase.h"

#include "world/WorldSetup.h"

namespace Project {
namespace Event {

class ReplaceWorldSetup : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & worldSetup;
    }
private:
    World::WorldSetup *worldSetup;
public:
    ReplaceWorldSetup() : worldSetup(0) {}
    ReplaceWorldSetup(World::WorldSetup *worldSetup) : worldSetup(worldSetup) {}
    
    World::WorldSetup *getWorldSetup() const { return worldSetup; }
    
    virtual EventType::type_t getType() const
        { return EventType::REPLACE_WORLD_SETUP; }
};

}  // namespace Event
}  // namespace Project

#endif
