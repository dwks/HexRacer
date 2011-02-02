#include "UpdateWorld.h"
#include "UpdateObject.h"

namespace Project {
namespace Event {

UpdateWorld::UpdateWorld(unsigned long milliseconds,
    Object::WorldManager *worldManager) : milliseconds(milliseconds) {
    
    Object::WorldManager::PlayerIteratorType it
        = worldManager->getPlayerIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        addUpdate(new UpdateObject(player->getID(),
            player->getTransformation(),
            player->getPhysicalObject()->getLinearVelocity(),
            player->getPhysicalObject()->getAngularVelocity()));
    }
}

UpdateWorld::~UpdateWorld() {
    for(int x = 0; x < int(updates.size()); x ++) {
        delete updates[x];
    }
}

}  // namespace Event
}  // namespace Project
