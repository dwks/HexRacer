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
    Object::Player *player = new Object::Player(id);
    
    playerList = new Object::PlayerList();
    playerList->addPlayer(player);
    
    // commented out until the ID is assigned correctly
    ADD_OBSERVER(new PlayerMovementHandler(this));
    ADD_OBSERVER(new UpdatePlayerListHandler(this));
}

PlayerManager::~PlayerManager() {
    delete playerList;
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
        
        if(renderable) {   // will always be true
            Math::Point origin = player->getPosition();
            Math::Matrix matrix = Math::Matrix::getTranslationMatrix(origin);
            
            renderable->getRenderProperties()->setTransformation(matrix);
            renderable->render(renderManager);
        }
    }
    
    /*static struct {
        float r, g, b;
    } colours[] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    };
    
    Object::PlayerList::IteratorType it = playerList->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        int c = player->getID() % (sizeof(colours) / sizeof(*colours));
        glColor3f(colours[c].r, colours[c].g, colours[c].b);
        
        //LOG(GLOBAL, "Player " << c << " at " << player->getPosition());
        
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        OpenGL::MathWrapper::glVertex(player->getPosition());
        glEnd();
        glPointSize(1.0f);
    }
    
    glColor3f(1.0f, 1.0f, 1.0f);*/
}

void PlayerManager::usePlayerList(Object::PlayerList *playerList) {
    delete this->playerList;
    this->playerList = playerList;
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
