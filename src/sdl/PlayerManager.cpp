#include "PlayerManager.h"

#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"

#include "event/ObserverList.h"

#include "log/Logger.h"

#include "render/RenderManager.h"
#include "render/MeshGroup.h"
#include "render/MeshLoader.h"
#include "render/ShaderUniformVector4.h"
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
		/*
        player->setPhysicalObject(
            Physics::PhysicsFactory::createPhysicalPlayer(
                manager->raceManager
                    ->startingPointForPlayer(player->getID()),
					manager->raceManager->startingPlayerDirection()));
		*/
		const Map::PathNode* node = player->getPathTracker()->getCurrentNode();
		Math::Point origin = node->getPosition();
		origin.setY(origin.getY() + 1.0);
		Math::Point direction = (node->getNextNodes()[0]->getPosition() - node->getPosition()).normalized();
		player->setPhysicalObject(
            Physics::PhysicsFactory::createPhysicalPlayer(origin, direction)
			);
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
