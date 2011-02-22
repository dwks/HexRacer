#include "AIManager.h"
#include "StraightDriver.h"

#include "object/Player.h"
#include "event/CreateObject.h"
#include "event/EventSystem.h"

namespace Project {
namespace AI {

void AIManager::createAIs(int count) {
    for(int ai = 0; ai < count; ai ++) {
        boost::shared_ptr<Driver> driver
            = boost::shared_ptr<Driver>(new StraightDriver());
        driverList.push_back(driver);
        
        Math::Point location = raceManager->startingPointForPlayer(ai);
        Math::Point direction = raceManager->startingPlayerDirection();
        
        Object::Player *player = new Object::Player(ai, location, direction);
        player->setPathTracker(new Map::PathTracker(*pathManager));
        
        EMIT_EVENT(new Event::CreateObject(player));
    }
}

}  // namespace AI
}  // namespace Project
