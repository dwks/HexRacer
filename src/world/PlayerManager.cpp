#include "PlayerManager.h"

#include "event/EventSystem.h"

#include "physics/PhysicsFactory.h"
#include "map/PathTracker.h"

#include "config.h"
#include "log/Logger.h"

namespace Project {
namespace World {

void PlayerManager::physicsTickHandler(Event::PhysicsTick *event) {
    Object::PlayerList::IteratorType it
        = worldManager->getPlayerList()->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        applyIntentions(player);
    }
}

void PlayerManager::warpOntoTrackHandler(Event::WarpOntoTrack *event) {
    warpPlayerOntoTrack(getPlayer(event->getPlayer()));
}

void PlayerManager::changeOfIntentionHandler(Event::ChangeOfIntention *event) {
    if(event->getIntention().getReset()) {
        getPlayer(event->getPlayer())->getPhysicalObject()
            ->getWarpTracker()->setRequestingHelp(true);
        
        //warpPlayerOntoTrack(getPlayer(event->getPlayer()));
    }
}

void PlayerManager::applyIntentions(Object::Player *player) {
    const PlayerIntention &intention = player->getIntention();
    
    if(intention.getAccel()) {
        player->applyAcceleration(intention.getAccel());
    }
    if(intention.getTurn()) {
        player->applyTurning(intention.getTurn());
    }
    if(intention.getJump()) {
        player->doJump();
    }
    /*if(intention.getReset()) {
        warpPlayerOntoTrack(player);
    }*/
}

void PlayerManager::warpPlayerOntoTrack(Object::Player *player) {
    delete player->getPhysicalObject();
    
    const Map::PathNode* node = player->getPathTracker()->getCurrentNode();
    if(node != NULL) {
        Math::Point origin = node->getPosition();
        Math::Point direction = (node->getNextNodes()[0]->getPosition() - node->getPosition()).normalized();
        
        origin.setY(origin.getY() + VEHICLE_WARP_Y_OFFSET);
        player->setPhysicalObject(
            Physics::PhysicsFactory::createPhysicalPlayer(origin, direction));
        
        LOG(WORLD, "Player " << player->getID()
            << " is off track, warping to " << origin);
    }
    else {
        Math::Point origin = raceManager->startingPointForPlayer(player->getID());
        Math::Point direction = Math::Point(0.0, 0.0, -1.0);
        
        origin.setY(origin.getY() + VEHICLE_WARP_Y_OFFSET);
        player->setPhysicalObject(
            Physics::PhysicsFactory::createPhysicalPlayer(origin, direction));
        
        LOG(WORLD, "Player " << player->getID()
            << " is off track, warping to starting position"
            << " [no path nodes]");
    }
}

PlayerManager::PlayerManager(int id, Object::WorldManager *worldManager)
    : id(id), worldManager(worldManager) {
    
    METHOD_OBSERVER(&PlayerManager::physicsTickHandler);
    METHOD_OBSERVER(&PlayerManager::warpOntoTrackHandler);
    METHOD_OBSERVER(&PlayerManager::changeOfIntentionHandler);
}

Object::Player *PlayerManager::getPlayer() {
    return worldManager->getPlayer(id);
}

Object::Player *PlayerManager::getPlayer(int id) {
    return worldManager->getPlayer(id);
}

}  // namespace World
}  // namespace Project
