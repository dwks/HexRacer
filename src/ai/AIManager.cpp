#include "AIManager.h"
#include "StraightDriver.h"
#include "DirectionalDriver.h"
#include "WeightedDriver.h"

#include "object/Player.h"
#include "event/CreateObject.h"
#include "event/EventSystem.h"

#include "world/WorldSetup.h"

namespace Project {
namespace AI {

void AIManager::physicsTickHandler(Event::PhysicsTick *event) {
    for(int ai = 0; ai < static_cast<int>(driverList.size()); ai ++) {
        int id = driverList[ai]->getPlayer()->getID();
        const World::PlayerIntention &intention = driverList[ai]->getAction();
        
        Object::Player *player = playerManager->getPlayer(id);
        bool identical = (intention == player->getIntention());
        if(!identical) {
            bool equal = (player->getIntention() == intention);
            player->setIntention(intention);
            
            if(!equal) {
                EMIT_EVENT(new Event::ChangeOfIntention(id, intention));
            }
        }
    }
}

AIManager::AIManager(Map::RaceManager *raceManager,
    Map::PathManager *pathManager, World::PlayerManager *playerManager,
	Paint::PaintManager* paintManager)
    : raceManager(raceManager), pathManager(pathManager),
    playerManager(playerManager), paintManager(paintManager) {
    
    METHOD_OBSERVER(&AIManager::physicsTickHandler);
}

int AIManager::createAIs(World::WorldSetup *worldSetup) {
    std::vector<int> ids;
    worldSetup->getAllPlayerIDs(ids);
    
    int count = 0;
    
    worldSetup->debugDump();
    
    for(std::vector<int>::size_type x = 0; x < ids.size(); x ++) {
        int ai = ids[x];
        if(worldSetup->getClientSettings(ai) != NULL) continue;
        
        count ++;
        
        LOG(NETWORK, "******** AI is controlling " << ai);
        
        Math::Point location = raceManager->startingPointForPlayer(ai);
        Math::Point direction = raceManager->startingPlayerDirection();
        
        World::WorldSetup::PlayerSettings *settings
            = worldSetup->getPlayerSettings(ai);
        
        Object::Player *player = new Object::Player(ai, location, direction);
        if(settings) {
            player->setPlayerName(settings->getName());
			player->setTeamID(settings->getColor());
        }
        
        player->setPathTracker(new Map::PathTracker(*pathManager));
		player->setRaceFinishIgnore(true);
        
		WeightedDriver* new_driver = new WeightedDriver(player);
		new_driver->setPaintManager(paintManager);
        boost::shared_ptr<Driver> driver
            = boost::shared_ptr<Driver>(new_driver);
        driverList.push_back(driver);
        
        EMIT_EVENT(new Event::CreateObject(player));
    }
    
    return count;
}

}  // namespace AI
}  // namespace Project
