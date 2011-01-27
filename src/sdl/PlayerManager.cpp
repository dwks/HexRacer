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

#include "settings/SettingsManager.h"
#include "config.h"

namespace Project {
namespace SDL {

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
    }
}

void PlayerManager::UpdatePlayerListHandler::observe(
    Event::UpdatePlayerList *update) {
    
    /*LOG(GLOBAL, "Replacing with PlayerList of "
        << update->getPlayerList()->getPlayerCount() << " players");*/
    manager->usePlayerList(update->getPlayerList());
}

PlayerManager::PlayerManager(int id, Object::WorldManager *worldManager)
    : id(id), worldManager(worldManager) {
    
    worldManager->initForClient(id);
    
    ADD_OBSERVER(new PlayerActionHandler(this));
    ADD_OBSERVER(new UpdatePlayerListHandler(this));
}

PlayerManager::~PlayerManager() {
    
}

void PlayerManager::applySuspension(Render::RenderManager *renderManager) {
    suspension.applySuspension(worldManager->getPlayerList(), renderManager);
}

/*void PlayerManager::render(Render::RenderManager *renderManager) {
    Object::PlayerList::IteratorType it
        = worldManager->getPlayerList()->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        player->getRenderableObject()->render(renderManager);
    }
}*/

void PlayerManager::usePlayerList(Object::PlayerList *playerList) {
    worldManager->usePlayerList(playerList);
}

Object::Player *PlayerManager::getPlayer() {
    return worldManager->getPlayerList()->getPlayer(id);
}

Object::Player *PlayerManager::getPlayer(int id) {
    return worldManager->getPlayerList()->getPlayer(id);
}

}  // namespace SDL
}  // namespace Project
