#include "PaintSubsystem.h"

#include "event/PaintEvent.h"
#include "event/ObserverList.h"

#include "config.h"

namespace Project {
namespace Paint {

void PaintSubsystem::TogglePaintingHandler::observe(
    Event::TogglePainting *toggle) {
    
    subsystem->setPainting(toggle->getID(), toggle->getPaintType());
}

PaintSubsystem::PaintSubsystem(Object::WorldManager *worldManager,
    Paint::PaintManager *paintManager, unsigned long tickTime)
    : TimedSubsystem(tickTime), worldManager(worldManager),
    paintManager(paintManager) {
    
    ADD_OBSERVER(new TogglePaintingHandler(this));
}

void PaintSubsystem::setPainting(int id,
    Event::TogglePainting::PaintType type) {
    
    painting[id] = type;
}

Event::TogglePainting::PaintType PaintSubsystem::getPainting(int id) {
    PaintingType::iterator found = painting.find(id);
    
    if(found == painting.end()) {
        return Event::TogglePainting::NOTHING;
    }
    else {
        return (*found).second;
    }
}

void PaintSubsystem::doAction(unsigned long currentTime) {
    for(PaintingType::iterator i = painting.begin(); i != painting.end();
        ++ i) {
        
        int id = (*i).first;
        Event::TogglePainting::PaintType type = (*i).second;
        
        // can't paint or erase when in the air
        if(!worldManager->getPlayer(id)->getOnGround()) continue;
        
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
    
    calculateBoostSpeeds();
}

void PaintSubsystem::calculateBoostSpeeds() {
    Object::WorldManager::PlayerIteratorType iterator
        = worldManager->getPlayerIterator();
    while(iterator.hasNext()) {
        Object::Player *player = iterator.next();
        
        if(getPainting(player->getID()) == Event::TogglePainting::NOTHING) {
            double factor = paintManager->weightedCellsInRadius(
                player->getPosition(),
                PAINTING_RADIUS,
                player->getID());
            
            //LOG(PAINT, "factor " << factor);
            
            player->setSpeedBoost(factor);
        }
        else {
            // if painting or erasing, slow down the player no matter what
            player->setSpeedBoost(0.8);
        }
    }
}

}  // namespace Paint
}  // namespace Project
