#include "PaintSubsystem.h"

#include "event/PaintEvent.h"
#include "event/ObserverList.h"

namespace Project {
namespace Paint {

void PaintSubsystem::TogglePaintingHandler::observe(
    Event::TogglePainting *toggle) {
    
    subsystem->setPainting(toggle->getID(), toggle->getPaintType());
}

PaintSubsystem::PaintSubsystem(Object::WorldManager *worldManager,
    unsigned long tickTime)
    : TimedSubsystem(tickTime), worldManager(worldManager) {
    
    ADD_OBSERVER(new TogglePaintingHandler(this));
}

void PaintSubsystem::setPainting(int id,
    Event::TogglePainting::PaintType type) {
    
    painting[id] = type;
}

void PaintSubsystem::doAction(unsigned long currentTime) {
    for(PaintingType::iterator i = painting.begin(); i != painting.end();
        ++ i) {
        
        static double PAINTING_RADIUS = 1.5;
        
        int id = (*i).first;
        Event::TogglePainting::PaintType type = (*i).second;
        
        switch(type) {
        case Event::TogglePainting::ERASING:
            EMIT_EVENT(new Event::PaintEvent(
                worldManager->getPlayer(id)->getPosition(),
                PAINTING_RADIUS,
                -1));
            break;
        case Event::TogglePainting::PAINTING:
            EMIT_EVENT(new Event::PaintEvent(
                worldManager->getPlayer(id)->getPosition(),
                PAINTING_RADIUS,
                id));
            break;
        default:
        case Event::TogglePainting::NOTHING:
            break;
        }
    }
}

}  // namespace Paint
}  // namespace Project
