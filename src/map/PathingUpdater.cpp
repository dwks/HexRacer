#include "PathingUpdater.h"
#include "physics/PhysicsWorld.h"

#include "settings/SettingsManager.h"

#include "event/EventSystem.h"
#include "event/PlayerAction.h"

namespace Project {
namespace Map {

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
            
            player->getPathTracker()->update(update_pos);
            
            // Start a new lap for the player if they have crossed the finish plane
            if (player->getPathTracker()->readyforNewLap() &&
                raceManager->getBoundingPlane().pointInside(origin_pos)) {
                    player->getPathTracker()->startNewLap();
                    LOG(WORLD, "Player: " << player->getID()
                        << " has finished lap "
                        << player->getPathTracker()->getNumLaps());

            }
        }
        else {
            //Reset the player if they are below the kill plane
            //Probably not doing the right ID check?
            if (origin_pos.getY() < raceManager->getKillPlaneY()) {
                EMIT_EVENT(new Event::PlayerAction(player->getID(),
                    Event::PlayerAction::FIX_OFF_TRACK, 0.0));
            }
        }
    }
}

}  // namespace Map
}  // namespace Project
