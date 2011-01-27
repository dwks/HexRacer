#ifndef PROJECT_PAINT__PAINT_SUBSYSTEM_H
#define PROJECT_PAINT__PAINT_SUBSYSTEM_H

#include <map>

#include "timing/TimedSubsystem.h"

#include "event/TogglePainting.h"
#include "event/TypedObserver.h"

#include "object/WorldManager.h"

namespace Project {
namespace Paint {

class PaintSubsystem : public Timing::TimedSubsystem {
private:
    class TogglePaintingHandler
        : public Event::TypedObserver<Event::TogglePainting> {
    private:
        PaintSubsystem *subsystem;
    public:
        TogglePaintingHandler(PaintSubsystem *subsystem)
            : subsystem(subsystem) {}
        
        virtual void observe(Event::TogglePainting *toggle);
    };
private:
    typedef std::map<int, Event::TogglePainting::PaintType> PaintingType;
    PaintingType painting;
    Object::WorldManager *worldManager;
public:
    PaintSubsystem(Object::WorldManager *worldManager, unsigned long tickTime);
    
    virtual void setPainting(int id, Event::TogglePainting::PaintType type);
    
    virtual void doAction(unsigned long currentTime);
};

}  // namespace Paint
}  // namespace Project

#endif
