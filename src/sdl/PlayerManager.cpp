#include "PlayerManager.h"

#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"

#include "event/ObserverList.h"

#include "log/Logger.h"

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

void PlayerManager::render() {
    static struct {
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
    
    glColor3f(1.0f, 1.0f, 1.0f);
}

void PlayerManager::usePlayerList(Object::PlayerList *playerList) {
    delete this->playerList;
    this->playerList = playerList;
}

Object::Player *PlayerManager::getPlayer() {
    return playerList->getPlayer(id);
}

}  // namespace SDL
}  // namespace Project
