#ifndef PROJECT_PAINT__PAINT_SUBSYSTEM_H
#define PROJECT_PAINT__PAINT_SUBSYSTEM_H

#include <map>

#include "timing/TimedSubsystem.h"

#include "event/TogglePainting.h"
#include "event/Enabler.h"

#include "paint/PaintManager.h"

#include "object/WorldManager.h"

namespace Project {
namespace Paint {

class PaintSubsystem : public Timing::TimedSubsystem, public Event::Enabler {
private:
    /*class TogglePaintingHandler
        : public Event::TypedObserver<Event::TogglePainting> {
    private:
        PaintSubsystem *subsystem;
    public:
        TogglePaintingHandler(PaintSubsystem *subsystem)
            : subsystem(subsystem) {}
        
        virtual void observe(Event::TogglePainting *toggle);
    };*/
private:
    typedef std::map<int, Event::TogglePainting::PaintType> PaintingType;
    PaintingType painting;
    Object::WorldManager *worldManager;
    Paint::PaintManager *paintManager;
protected:
    void togglePaintingObserver(Event::TogglePainting *toggle);
public:
    PaintSubsystem(Object::WorldManager *worldManager,
        Paint::PaintManager *paintManager, unsigned long tickTime);
    
    virtual void setPainting(int id, Event::TogglePainting::PaintType type);
    
    virtual Event::TogglePainting::PaintType getPainting(int id);
    
    virtual void doAction(unsigned long currentTime);
private:
    void calculateBoostSpeeds();
};

}  // namespace Paint
}  // namespace Project

#endif
