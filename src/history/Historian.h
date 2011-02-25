#ifndef PROJECT_HISTORY__HISTORIAN_H
#define PROJECT_HISTORY__HISTORIAN_H

#include "PingTimeMeasurer.h"

#include "object/WorldManager.h"
#include "event/UpdateWorld.h"
#include "physics/PhysicsWorld.h"

#include "event/Enabler.h"
#include "event/MultiObserver.h"

namespace Project {
namespace History {

class Historian : public Event::Enabler {
private:
    class WorldHandler : public Event::MultiObserver {
    private:
        Object::WorldManager *worldManager;
        Historian *historian;
    public:
        WorldHandler(Object::WorldManager *worldManager, Historian *historian)
            : worldManager(worldManager), historian(historian) {}
        
        virtual void observe(Event::EventBase *event);
        virtual bool interestedIn(Event::EventType::type_t type);
    };
private:
    Object::WorldManager *worldManager;
    Physics::PhysicsWorld *physicsWorld;
    PingTimeMeasurer *pingTime;
public:
    Historian();
    ~Historian();
    
    void setWorldManager(Object::WorldManager *worldManager);
    void setPhysicsWorld(Physics::PhysicsWorld *physicsWorld);
    
    void handleUpdateWorld(Event::UpdateWorld *updateWorld);
    bool updateWorldTooOld(Event::UpdateWorld *updateWorld);
    void advanceWorld(Event::UpdateWorld *updateWorld);
};

}  // namespace History
}  // namespace Project

#endif
