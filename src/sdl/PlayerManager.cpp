#include "PlayerManager.h"

#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"

#include "event/ObserverList.h"

namespace Project {
namespace SDL {

void PlayerManager::PlayerMovementHandler::observe(
    Event::PlayerMovement *movement) {
    
    player->addPosition(movement->getMovement());
}

PlayerManager::PlayerManager(int id) : id(id) {
    Object::Player *player = new Object::Player(id, Math::Point());
    
    playerList = new Object::PlayerList();
    playerList->addPlayer(player);
    
    ADD_OBSERVER(new PlayerMovementHandler(player));
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
        
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        OpenGL::MathWrapper::glVertex(player->getPosition());
        glEnd();
        glPointSize(1.0f);
    }
    
    glColor3f(1.0f, 1.0f, 1.0f);
}

}  // namespace SDL
}  // namespace Project
