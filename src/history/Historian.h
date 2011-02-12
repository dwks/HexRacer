#ifndef PROJECT_HISTORY__HISTORIAN_H
#define PROJECT_HISTORY__HISTORIAN_H

#include "PingTimeMeasurer.h"

#include "object/WorldManager.h"
#include "event/UpdateWorld.h"

#include "event/MultiObserver.h"

namespace Project {
namespace History {

class Historian {
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
    PingTimeMeasurer *pingTime;
public:
    Historian(Object::WorldManager *worldManager);
    ~Historian();
    
    void handleUpdateWorld(Event::UpdateWorld *updateWorld);
};

}  // namespace History
}  // namespace Project

#endif
