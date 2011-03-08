#include "PathingUpdater.h"
#include "physics/PhysicsWorld.h"

#include "settings/SettingsManager.h"

#include "event/EventSystem.h"
#include "event/PlayerProgressEvent.h"

namespace Project {
namespace Map {

PathingUpdater::PathingUpdater(
    boost::shared_ptr<Object::WorldManager> worldManager,
    boost::shared_ptr<RaceManager> raceManager,
    boost::shared_ptr<World::PlayerManager> playerManager) {
    
    this->worldManager = worldManager;
    this->raceManager = raceManager;
    this->playerManager = playerManager;
    
    warpDetector = new WarpDetector(raceManager.get());
}

PathingUpdater::~PathingUpdater() {
    delete warpDetector;
}

void PathingUpdater::update() {
    if(!GET_SETTING("game.enablepathing", true)) {
        return;
    }
    
    Object::WorldManager::PlayerIteratorType iterator = worldManager->getPlayerIterator();
    while (iterator.hasNext()) {
        Object::Player* player = iterator.next();
        
        // Raycast downward to find the update point
        Math::Point origin_pos = player->getPosition();
        Math::Point dir_pos = origin_pos;
        dir_pos.setY(dir_pos.getY() - VEHICLE_PATH_RAY_MAX_HEIGHT);
        
        Math::Point update_pos;
        
        Physics::PhysicsWorld *physicsWorld
            = Physics::PhysicsWorld::getInstance();
        
        // Update if the player is above the track
        if(physicsWorld->raycastPoint(origin_pos, dir_pos, &update_pos)) {
            if(!player->getPathTracker()) continue;  // no path tracker
            if(!player->getPathTracker()->atLeastOneNode()) continue;
            
            player->getPathTracker()->update(update_pos);
            
            // Start a new lap for the player if they have crossed the finish plane
            if (player->getPathTracker()->readyforNewLap() &&
                raceManager->getBoundingPlane().pointInside(origin_pos)) {
                    player->getPathTracker()->startNewLap();
                    LOG(WORLD, "Player: " << player->getID()
                        << " has finished lap "
                        << player->getPathTracker()->getNumLaps());
                
                if(playerManager->getPlayer() == player) {
                    EMIT_EVENT(new Event::PlayerProgressEvent(
                        player->getPathTracker()->getNumLaps(),
                        player->getPathTracker()->getLapProgress()));
                }
            }
        }
        
        warpDetector->checkForWarping(player);
    }
}

}  // namespace Map
}  // namespace Project