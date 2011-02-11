#include "PlayerManager.h"

#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"

#include "event/ObserverList.h"

#include "log/Logger.h"

#include "render/RenderManager.h"
#include "mesh/MeshGroup.h"
#include "mesh/MeshLoader.h"
#include "shader/ShaderParamVector4.h"
#include "render/RenderList.h"
#include "render/ColorConstants.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "map/PathTracker.h"

#include "settings/SettingsManager.h"
#include "settings/ProgramSettings.h"
#include "config.h"

namespace Project {
namespace SDL {

void PlayerManager::PlayerActionHandler::observe(
    Event::PlayerAction *action) {
    
    /*if(Settings::ProgramSettings::getInstance()->isConnectedClient()
        && action->getMovementType() != Event::PlayerAction::FIX_OFF_TRACK) {
        
        return;
    }*/
    
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
        }
        else {
            Math::Point origin = manager->raceManager->startingPointForPlayer(player->getID());
            Math::Point direction = Math::Point(0.0, 0.0, -1.0);
            
            origin.setY(origin.getY() + VEHICLE_RESET_Y_OFFSET);
            player->setPhysicalObject(
                Physics::PhysicsFactory::createPhysicalPlayer(origin, direction));
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

}  // namespace SDL
}  // namespace Project
