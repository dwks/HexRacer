#include "AIManager.h"
#include "StraightDriver.h"
#include "DirectionalDriver.h"
#include "WeightedDriver.h"

#include "object/Player.h"
#include "event/CreateObject.h"
#include "event/EventSystem.h"

namespace Project {
namespace AI {

void AIManager::physicsTickHandler(Event::PhysicsTick *event) {
    for(int ai = 0; ai < static_cast<int>(driverList.size()); ai ++) {
        const World::PlayerIntention &intention = driverList[ai]->getAction();
        
        Object::Player *player = playerManager->getPlayer(ai);
        bool identical = (intention == player->getIntention());
        if(!identical) {
            player->setIntention(intention);
            
            EMIT_EVENT(new Event::ChangeOfIntention(
                ai,
                intention));
        }
    }
}

AIManager::AIManager(Map::RaceManager *raceManager,
    Map::PathManager *pathManager, World::PlayerManager *playerManager)
    : raceManager(raceManager), pathManager(pathManager),
    playerManager(playerManager) {
    
    METHOD_OBSERVER(&AIManager::physicsTickHandler);
}

void AIManager::createAIs(int count) {
    for(int ai = 0; ai < count; ai ++) {
        Math::Point location = raceManager->startingPointForPlayer(ai);
        Math::Point direction = raceManager->startingPlayerDirection();
        
        Object::Player *player = new Object::Player(ai, location, direction);
        player->setPathTracker(new Map::PathTracker(*pathManager));
        
        boost::shared_ptr<Driver> driver
            = boost::shared_ptr<Driver>(new WeightedDriver(player));
        driverList.push_back(driver);
        
        EMIT_EVENT(new Event::CreateObject(player));
    }
}

}  // namespace AI
}  // namespace Project
