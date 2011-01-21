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

#include "physics/PhysicsWorld.h"

#include "settings/SettingsManager.h"
#include "config.h"

namespace Project {
namespace SDL {

void PlayerManager::PlayerMovementHandler::observe(
    Event::PlayerMovement *movement) {
    
    manager->getPlayer()->applyMovement(movement->getMovement());
}

void PlayerManager::UpdatePlayerListHandler::observe(
    Event::UpdatePlayerList *update) {
    
    /*LOG(GLOBAL, "Replacing with PlayerList of "
        << update->getPlayerList()->getPlayerCount() << " players");*/
    manager->usePlayerList(update->getPlayerList());
}

PlayerManager::PlayerManager(int id) : id(id) {
    Object::Player *player = new Object::Player(id, INITIAL_CAR_LOCATION);
    
    playerList = new Object::PlayerList();
    playerList->addPlayer(player);
    
    // commented out until the ID is assigned correctly
    ADD_OBSERVER(new PlayerMovementHandler(this));
    ADD_OBSERVER(new UpdatePlayerListHandler(this));
}

PlayerManager::~PlayerManager() {
    delete playerList;
}

void PlayerManager::applySuspension(Render::RenderManager *renderManager) {
    if(!GET_SETTING("physics.driving.dosuspension", 0)) {
        return;
    }
    
    Object::PlayerList::IteratorType it = playerList->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        static const Math::Point suspensionPoint[] = {
            Math::Point(5.0, -2.0, 10.0) / 20.0,
            Math::Point(-5.0, -2.0, 10.0) / 20.0,
            Math::Point(-5.0, -2.0, -10.0) / 20.0,
            Math::Point(5.0, -2.0, -10.0) / 20.0
        };
        
        for(int wheel = 0; wheel < 4; wheel ++) {
            Math::Matrix matrix = player->getTransformation();
            matrix = Math::Matrix::getTranslationMatrix(
                suspensionPoint[wheel]) * matrix;
            
            Math::Point axis = matrix * Math::Point(0.0, -1.0, 0.0, 0.0);
            Math::Point point = matrix * Math::Point(0.0, 0.0, 0.0, 1.0);
            
            //LOG(OPENGL, "Suspension point: " << point);
            
            static const double REST_LENGTH = 1.0;
            
            double length = Physics::PhysicsWorld::getInstance()
                ->raycastLength(point, point + axis);
            
            length = REST_LENGTH - length;
            if(length < -1.0) length = -1.0;
            if(length > +1.0) length = +1.0;
            
            static const double K = 10.0;
            double factor = -K * length;
            
            //factor = (factor + 4.5) * 0.5 - 4.5;
            
            //LOG(PHYSICS, "crazy force: " << factor);
            player->applyForce(axis * factor, suspensionPoint[wheel]);
            
            if(0) {
                Render::MeshGroup* player_cube_mesh
                    = Render::MeshLoader::getInstance()->getModelByName("playerCube");
                Render::RenderList *renderList = new Render::RenderList();
                renderList->addRenderable(player_cube_mesh);
                
                renderList->getRenderProperties()->addShaderParameter(
                    new Render::ShaderUniformVector4(
                        "playerColor", getPlayerColor(player->getID())));
                
                renderList->getRenderProperties()->setTransformation(
                    Math::Matrix::getTranslationMatrix(point));
                renderList->render(renderManager);
            }
        }
    }
}

void PlayerManager::render(Render::RenderManager *renderManager) {
    Object::PlayerList::IteratorType it = playerList->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        Render::RenderableObject *renderable = player->getRenderableObject();
        if(!renderable) {
            Render::RenderList *renderList = new Render::RenderList();
            renderable = renderList;
            
            Render::MeshGroup* player_cube_mesh
                = Render::MeshLoader::getInstance()->getModelByName("playerCube");
            renderList->addRenderable(player_cube_mesh);
            
            renderList->getRenderProperties()->addShaderParameter(
				new Render::ShaderUniformVector4("playerColor", getPlayerColor(player->getID())));
        }
        
        // first if, then second if
        
        if(renderable) {
            //Math::Point origin = player->getPosition();
            //Math::Matrix matrix = Math::Matrix::getTranslationMatrix(origin);
            Math::Matrix matrix = player->getTransformation();
            
            // original size of model is 2x2, scale appropriately
            matrix = matrix
                * Math::Matrix::getScalingMatrix(Math::Point(5.0, 2.0, 10.0) / 2.0);
            
			//btRigidBody* body = player->getPhysicalObject()->getPrimaryRigidBody();
			//btQuaternion quat = body->getOrientation();
            
            renderable->getRenderProperties()->setTransformation(matrix);
            renderable->render(renderManager);
        }
    }
}

void PlayerManager::usePlayerList(Object::PlayerList *playerList) {
#if 1
    delete this->playerList;
    this->playerList = playerList;
#else
    Object::PlayerList::IteratorType it = playerList->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        //player->getPhysicalObject()->
    }
#endif
}

Object::Player *PlayerManager::getPlayer() {
    return playerList->getPlayer(id);
}

OpenGL::Color PlayerManager::getPlayerColor(int id) {
	switch (id % 4) {
		case 0: return OpenGL::Color::RED;
		case 1: return OpenGL::Color::GREEN;
		case 2: return OpenGL::Color::BLUE;
		case 3: return OpenGL::Color::YELLOW;
	}
	return OpenGL::Color::WHITE;
}

}  // namespace SDL
}  // namespace Project
