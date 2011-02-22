#include "PlayerManager.h"

#include "event/EventSystem.h"

#include "physics/PhysicsFactory.h"
#include "map/PathTracker.h"

#include "config.h"
#include "log/Logger.h"

namespace Project {
namespace World {

void PlayerManager::PlayerActionHandler::observe(
    Event::PlayerAction *action) {
    
    Object::Player *player = manager->getPlayer();
    
    switch(action->getMovementType()) {
    case Event::PlayerAction::ACCELERATE:
        player->applyAcceleration(action->getValue());
        break;
    case Event::PlayerAction::TURN:
        player->applyTurning(action->getValue());
        break;
    case Event::PlayerAction::JUMP:
        player->doJump();
        break;
    case Event::PlayerAction::FIX_OFF_TRACK:
        delete player->getPhysicalObject();

        const Map::PathNode* node = player->getPathTracker()->getCurrentNode();
        if(node != NULL) {
            Math::Point origin = node->getPosition();
            Math::Point direction = (node->getNextNodes()[0]->getPosition() - node->getPosition()).normalized();
            
            origin.setY(origin.getY() + VEHICLE_RESET_Y_OFFSET);
            player->setPhysicalObject(
                Physics::PhysicsFactory::createPhysicalPlayer(origin, direction));
            
            LOG(WORLD, "Player " << player->getID()
                << " is off track, warping to " << origin);
        }
        else {
            Math::Point origin = manager->raceManager->startingPointForPlayer(player->getID());
            Math::Point direction = Math::Point(0.0, 0.0, -1.0);
            
            origin.setY(origin.getY() + VEHICLE_RESET_Y_OFFSET);
            player->setPhysicalObject(
                Physics::PhysicsFactory::createPhysicalPlayer(origin, direction));
            
            LOG(WORLD, "Player " << player->getID()
                << " is off track, warping to starting position"
                << " [no path nodes]");
        }
        
        break;
    }
}

PlayerManager::PlayerManager(int id, Object::WorldManager *worldManager)
    : id(id), worldManager(worldManager) {
    
    ADD_OBSERVER(new PlayerActionHandler(this));
}

Object::Player *PlayerManager::getPlayer() {
    return worldManager->getPlayer(id);
}

Object::Player *PlayerManager::getPlayer(int id) {
    return worldManager->getPlayer(id);
}

}  // namespace World
}  // namespace Project
